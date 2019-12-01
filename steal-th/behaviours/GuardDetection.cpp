#include "GuardDetection.h"
#include "../scenes/LevelScene.h"
#include "../gameobjects/Tile.h"
#include "PlayerMovement.h"

#include <engine/debugbt/DebugLog.h>
#include <engine/behaviours/SpotLight.h>
#include <engine/GameObject.h>
#include <engine/sound/SoundManager.h>
#include <engine/sound/Music.h>
#include <engine/sound/Sound.h>
#include <vector>
#include <cmath>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>


namespace {
    /* 
    * Implementation of Bresenham's line algorithm
    * Arguments: int x0, int y0, int x1, int y1
    * x0 = The guard's position x.
    * y0 = The guard's position y.
    * x1 = The guard's max view position x.
    * y1 = The guard's max view position y.
    * Function returns a list of tiles.
    */

    void PlotLineLow(int x0, int y0, int x1, int y1, std::vector<glm::ivec2> &output) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1;

        if(dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int D = 2 * dy - dx;
        int y = y0;

        for(int x = x0; x <= x1; x++) {
            output.push_back(glm::ivec2(x,y));
            
            if(D > 0 ) {
                y = y + yi;
                D = D - 2 * dx;
            }
            
            D = D + 2 * dy;
        }
    }

     void PlotLineHigh(int x0, int y0, int x1, int y1, std::vector<glm::ivec2> &output) {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = 1;

        if(dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int D = 2 * dx - dy;
        int x = x0;

        for(int y = y0; y <= y1; y++) {
            output.push_back(glm::ivec2(x,y));
            
            if(D > 0) {
                x = x + xi;
                D = D - 2 * dy;
            }
            
            D = D + 2 * dx;
        }
	 }

	 // Returns true if input coordinates were reversed, otherwise returns false.
	 bool PlotLine(int x0, int y0, int x1, int y1, std::vector<glm::ivec2> &output) {
		 if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
			 if (x0 > x1) {
				 PlotLineLow(x1, y1, x0, y0, output);
				 return true;
			 }
			 else {
				 PlotLineLow(x0, y0, x1, y1, output);
				 return false;
			 }
		 }
		 else {
			 if (y0 > y1) {
				 PlotLineHigh(x1, y1, x0, y0, output);
				 return true;
			 }
			 else {
				 PlotLineHigh(x0, y0, x1, y1, output);
				 return false;
			 }
		 }
	 }

	 const int NUM_VALUES_PER_POINT{ 2 };

	 // TODO: maybe move this flag to a more appropriate location?
	 const bool IS_OPEN_CL_ENABLED{ true };
}

std::unique_ptr<OpenCLManager> GuardDetection::m_openCLManager;

GuardDetection::GuardDetection(int guardIndex, LevelScene* levelScene, 
	GameObject* playerObj, float maxViewDist, int tileViewRadius) :
	Behaviour(BehaviourType::GuardDetection),
	m_levelScene(levelScene), m_playerObj(playerObj),
	m_maxViewDist(maxViewDist), m_tileViewRadius(tileViewRadius),
	m_numDetectionRays(2 * tileViewRadius + 1)
{
	if (IS_OPEN_CL_ENABLED)
	{
		cl_mem outputBuffer{ m_openCLManager->CreateOutputBuffer(sizeof(bool) * m_numDetectionRays) };
		m_openCLManager->SetKernelArg(m_guardIndex, 2, sizeof(cl_mem), &outputBuffer);

		m_openCLManager->SetKernelArg(m_guardIndex, 6, sizeof(float), &LevelScene::TILE_SIZE);

		glm::ivec2 levelSize{ m_levelScene->GetLevelSize() };
		m_openCLManager->SetKernelArg(m_guardIndex, 7, sizeof(int), &levelSize.x);
		m_openCLManager->SetKernelArg(m_guardIndex, 8, sizeof(int), &levelSize.y);

		std::vector<int> tiles;
		m_levelScene->GetTiles(tiles);
		cl_mem tilesBuffer{ m_openCLManager->CreateInputBuffer(sizeof(int) * tiles.size(), &tiles[0]) };
		m_openCLManager->SetKernelArg(m_guardIndex, 9, sizeof(cl_mem), &tilesBuffer);

		const int MAX_VIEW_DIST_TILES{ static_cast<int>(m_maxViewDist / LevelScene::TILE_SIZE) };
		m_openCLManager->SetKernelArg(m_guardIndex, 13, sizeof(int) * NUM_VALUES_PER_POINT * MAX_VIEW_DIST_TILES * 2, NULL);

		m_outputBuffer = new bool[m_numDetectionRays];
	}
}

GuardDetection::~GuardDetection()
{
	delete[] m_outputBuffer;
}

void GuardDetection::Update(float deltaTime)
{
	// Reset the detected flag.
	m_isPlayerDetected = false;

	// Don't bother continuing if we can't even get the tile data.
	if (m_levelScene == nullptr)
		return;

	// Only call one of these.
	if (IS_OPEN_CL_ENABLED)
	{
		UpdateOpenCL();
	}
	else
	{
		UpdateSerial();
	}

	// Set colour of guard spot light depending on player detection.
	std::weak_ptr<SpotLight> guardCone = std::static_pointer_cast<SpotLight>(gameObject->GetBehaviour(BehaviourType::SpotLight).lock());
	if (m_isPlayerDetected)
	{
		if (!guardCone.expired()) 
		{
			guardCone.lock()->SetColour(glm::vec3(10.0f, 0.0f, 0.0));
		}

		// Stop player movement.
		std::shared_ptr<PlayerMovement> playerMovement{ m_playerObj->GetBehaviour<PlayerMovement>().lock() };
		playerMovement->ResetVelocity();

		SoundManager::getInstance().getSound("detected")->play();
		SoundManager::getInstance().getMusic("music")->stop();
		SoundManager::getInstance().getSound("lose")->play();
		m_levelScene->stopUpdates();
		DebugLog::Info("Detected. Game over");
		// NEED UI!!!
	}
	else 
	{
		if (!guardCone.expired()) 
		{
			guardCone.lock()->SetColour(glm::vec3(10.0f));
		}
	}

	// Reset the player collision flag.
	m_isCollidingPlayer = false;
}

void GuardDetection::Draw(Shader& shader) {}

bool GuardDetection::HandleMessage(unsigned int senderID, std::string& message) {
    return false;
}

void GuardDetection::OnCollisionStay(GLuint other)
{
	GameObject* otherObj{ m_levelScene->GetWorldGameObjectById(other) };
	if (otherObj == m_playerObj)
	{
		m_isCollidingPlayer = true;
	}
}

void GuardDetection::InitOpenCL()
{
	if (IS_OPEN_CL_ENABLED)
	{
		if (m_openCLManager == nullptr)
			m_openCLManager = std::make_unique<OpenCLManager>("../Assets/opencl/guard_detection.cl", "guard_detection");
		else
			m_openCLManager->ReleasePrograms();
	}
}

void GuardDetection::UpdateOpenCL()
{
	// Update these kernel arguments.
	std::vector<float> endpointsX, endpointsZ;
	GetDetectionRayEndPoints(endpointsX, endpointsZ);
	cl_mem endpointsXBuffer{ m_openCLManager->CreateInputBuffer(sizeof(float) * m_numDetectionRays, &endpointsX[0]) };
	m_openCLManager->SetKernelArg(m_guardIndex, 0, sizeof(cl_mem), &endpointsXBuffer);
	cl_mem endpointsZBuffer{ m_openCLManager->CreateInputBuffer(sizeof(float) * m_numDetectionRays, &endpointsZ[0]) };
	m_openCLManager->SetKernelArg(m_guardIndex, 1, sizeof(cl_mem), &endpointsZBuffer);

	m_openCLManager->SetKernelArg(m_guardIndex, 3, sizeof(float), &gameObject->position.x);
	m_openCLManager->SetKernelArg(m_guardIndex, 4, sizeof(float), &gameObject->position.z);
	m_openCLManager->SetKernelArg(m_guardIndex, 5, sizeof(float), &gameObject->rotation.y);

	m_openCLManager->SetKernelArg(m_guardIndex, 10, sizeof(float), &m_playerObj->position.x);
	m_openCLManager->SetKernelArg(m_guardIndex, 11, sizeof(float), &m_playerObj->position.z);

	m_openCLManager->SetKernelArg(m_guardIndex, 12, sizeof(int), &m_isCollidingPlayer);

	// Run OpenCL kernel for detection.
	size_t globalWorkSize[1]{ static_cast<size_t>(m_numDetectionRays) };
	size_t localWorkSize[1]{ 1 };
	m_openCLManager->EnqueueKernel(m_guardIndex, 1, globalWorkSize, localWorkSize);

	// Get the results from OpenCL.
	m_openCLManager->ReadOutput(&m_outputBuffer[0], sizeof(bool) * m_numDetectionRays);

	m_openCLManager->ReleaseMemoryObject(endpointsXBuffer);
	m_openCLManager->ReleaseMemoryObject(endpointsZBuffer);

	for (int i = 0; i < m_numDetectionRays; ++i)
	{
		m_isPlayerDetected |= m_outputBuffer[i];

		if (m_isPlayerDetected)
		{
			break;
		}
	}
}

void GuardDetection::UpdateSerial()
{
	// The guard's tile position is point 1 for the line algorithm.
	// We want this to be the tile directly in front of the guard.
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };

	// For rotation == 0.f, the guard is facing down.
	glm::vec2 unrotatedMaxDistPoint{ guardWorldPos.x, guardWorldPos.y + m_maxViewDist };

	// Check if the guard can detect the player at its maximum view distance.
	int numEndTiles{ m_tileViewRadius * 2 };
	for (int i = 0; i < numEndTiles + 1; ++i)
	{
		glm::vec2 pos{ unrotatedMaxDistPoint };
		pos.x -= (m_tileViewRadius * LevelScene::TILE_SIZE);
		pos.x += (i * LevelScene::TILE_SIZE);
		m_isPlayerDetected |= TryDetectPlayer(guardWorldPos, pos);
	}
}

void GuardDetection::GetDetectionRayEndPoints(std::vector<float>& endpointsX,
	std::vector<float>& endpointsZ) const
{
	// The guard's tile position is point 1 for the line algorithm.
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };

	// For rotation == 0.f, the guard is facing down.
	glm::vec2 unrotatedMaxDistPoint{ guardWorldPos.x, guardWorldPos.y + m_maxViewDist };

	// Check if the guard can detect the player at its maximum view distance.
	for (int i = 0; i < m_numDetectionRays; ++i)
	{
		glm::vec2 pos{ unrotatedMaxDistPoint };
		pos.x -= (m_tileViewRadius * LevelScene::TILE_SIZE);
		pos.x += (i * LevelScene::TILE_SIZE);

		endpointsX.push_back(pos.x);
		endpointsZ.push_back(pos.y);
	}
}

bool GuardDetection::TryDetectPlayer(glm::vec2 startPoint, glm::vec2 maxDistancePoint)
{
	// Rotate the vector (start point to max distance point) by
	// the guard's rotation.
	glm::vec2 viewVector{ maxDistancePoint - startPoint };
	viewVector = glm::rotate(viewVector, -gameObject->rotation.y);

	// Rotate the vector (guard's position to start point) by
	// the guard's rotation.
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };
	glm::vec2 startVector{ startPoint - guardWorldPos };
	startVector = glm::rotate(startVector, -gameObject->rotation.y);

	// Get the new rotated start point and use it as point 1 for the 
	// line algorithm.
	glm::vec2 newStartPoint{ guardWorldPos + startVector };

	// The max tile distance point is point 2 for the line algorithm.
	glm::vec2 maxWorldDistPoint{ newStartPoint + viewVector };
	glm::ivec2 maxTileDistPoint{ m_levelScene->GetTileCoordFromPos(maxWorldDistPoint) };
	glm::ivec2 levelSize{ m_levelScene->GetLevelSize() };
	maxTileDistPoint.x = glm::clamp(maxTileDistPoint.x, 0, levelSize.x - 1);
	maxTileDistPoint.y = glm::clamp(maxTileDistPoint.y, 0, levelSize.y - 1);

	// If the starting point is outside of the level, then just cancel this 
	// detection check.
	glm::ivec2 guardTilePos{ m_levelScene->GetTileCoordFromPos(newStartPoint) };
	if (guardTilePos.x < 0 || guardTilePos.x > levelSize.x - 1 ||
		guardTilePos.y < 0 || guardTilePos.y > levelSize.y - 1)
	{
		return false;
	}

	// Call the line algorithm to get the points on the line.
	std::vector<glm::ivec2> output;
	bool isUsingLow{ PlotLine(guardTilePos.x, guardTilePos.y, maxTileDistPoint.x, maxTileDistPoint.y, output) };

	glm::vec2 playerWorldPos{ glm::vec2(m_playerObj->position.x, m_playerObj->position.z) };
	glm::ivec2 playerTilePos{ m_levelScene->GetTileCoordFromPos(playerWorldPos) };

	// Check points on the line, ordered from closest point to the 
	// guard to furthest.
	// The output of the line algorithm has its points in an order
	// that depends on whether the input coordinates were reversed.
	auto currentIt{ isUsingLow ? output.end() - 1 : output.begin() };
	size_t currentCount{ 0 };
	while (true)
	{
		glm::ivec2 point{ *currentIt };
		//DebugLog::Info(std::to_string(point.x) + ", " + std::to_string(point.y));

		// Check if the vision ray has hit a wall yet.
		TileType tile{ m_levelScene->GetTile(point.x, point.y) };
		if (tile == TileType::WALL ||
			tile == TileType::RED_DOOR ||
			tile == TileType::GREEN_DOOR ||
			tile == TileType::BLUE_DOOR)
		{
			return false;
		}

		// Check if the player is on this tile.
		if (point == playerTilePos)
		{
			// If the player was detected while on the same tile as this guard,
			// make sure they are actually colliding for the detection to count.
			// Otherwise, just disable the detection.
			return !(playerTilePos == guardTilePos && !m_isCollidingPlayer);
		}

		// Check adjacent walls to handle corner cases, when looking 
		// diagonally through this tile.
		if (currentCount + 1 < output.size())
		{
			glm::ivec2 nextPoint{ *(currentIt + (isUsingLow ? -1 : 1)) };
			glm::ivec2 diff{ nextPoint - point };

			TileType cornerTile{ TileType::FLOOR };
			TileType cornerTile2{ TileType::FLOOR };

			// Next point is moving toward top-left.
			if (diff.x < 0 && diff.y < 0)
			{
				cornerTile = m_levelScene->GetTile(point.x - 1, point.y);
				cornerTile2 = m_levelScene->GetTile(point.x, point.y - 1);
			}
			// Next point is moving toward top-right.
			else if (diff.x > 0 && diff.y < 0)
			{
				cornerTile = m_levelScene->GetTile(point.x + 1, point.y);
				cornerTile2 = m_levelScene->GetTile(point.x, point.y - 1);
			}
			// Next point is moving toward bottom-left.
			else if (diff.x < 0 && diff.y > 0)
			{
				cornerTile = m_levelScene->GetTile(point.x - 1, point.y);
				cornerTile2 = m_levelScene->GetTile(point.x, point.y + 1);
			}
			// Next point is moving toward bottom-right.
			else if (diff.x > 0 && diff.y > 0)
			{
				cornerTile = m_levelScene->GetTile(point.x + 1, point.y);
				cornerTile2 = m_levelScene->GetTile(point.x, point.y + 1);
			}

			bool isBlockingCorner1{ cornerTile == TileType::WALL ||
				cornerTile == TileType::RED_DOOR ||
				cornerTile == TileType::GREEN_DOOR ||
				cornerTile == TileType::BLUE_DOOR };

			bool isBlockingCorner2{ cornerTile2 == TileType::WALL ||
				cornerTile2 == TileType::RED_DOOR ||
				cornerTile2 == TileType::GREEN_DOOR ||
				cornerTile2 == TileType::BLUE_DOOR };

			if (isBlockingCorner1 || isBlockingCorner2)
			{
				return false;
			}
		}

		// Done checking all points.
		if (++currentCount == output.size())
			return false;

		// Otherwise, move the iterator to the next point.
		currentIt += (isUsingLow ? -1 : 1);
	}
}
