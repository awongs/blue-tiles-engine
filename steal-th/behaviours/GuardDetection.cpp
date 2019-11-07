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
#include <engine/behaviours/TextBehaviour.h>
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
}

GuardDetection::GuardDetection(LevelScene *levelScene, GameObject *playerObj,
	float maxViewDist, int tileViewRadius) :
	Behaviour(BehaviourType::GuardDetection),
	m_levelScene(levelScene), m_playerObj(playerObj), 
	m_maxViewDist(maxViewDist), m_tileViewRadius(tileViewRadius)
{
	m_isPlayerDetected = false;
}

void GuardDetection::Update(float deltaTime)
{
	// Reset the detected flag.
	m_isPlayerDetected = false;

	// Don't bother continuing if we can't even get the tile data.
	if (m_levelScene == nullptr)
		return;

	// The guard's tile position is point 1 for the line algorithm.
	// We want this to be the tile directly in front of the guard.
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };
	glm::vec2 frontWorldPos{ guardWorldPos.x, guardWorldPos.y + LevelScene::TILE_SIZE };

	// For rotation == 0.f, the guard is facing down.
	glm::vec2 unrotatedMaxDistPoint{ frontWorldPos.x, frontWorldPos.y + m_maxViewDist };

	// Check if the guard can detect the player at its maximum view distance.
	int numEndTiles{ m_tileViewRadius * 2 };
	for (int i = 0; i < numEndTiles + 1; ++i)
	{
		glm::vec2 pos{ unrotatedMaxDistPoint };
		pos.x -= (m_tileViewRadius * LevelScene::TILE_SIZE);
		pos.x += (i * LevelScene::TILE_SIZE);
		m_isPlayerDetected |= tryDetectPlayer(guardWorldPos, pos);
	}

	if (m_isPlayerDetected)
	{
		// If the the player was detected while on the same tile as this guard,
		// make sure they are actually colliding for the detection to count.
		// Otherwise, just disable the detection.
		glm::vec2 playerPos{ m_playerObj->position.x, m_playerObj->position.z };
		glm::ivec2 playerTilePos{ m_levelScene->GetTileCoordFromPos(playerPos) };
		glm::ivec2 guardTilePos{ m_levelScene->GetTileCoordFromPos(guardWorldPos) };
		if (playerTilePos == guardTilePos && !m_isCollidingPlayer)
		{
			m_isPlayerDetected = false;
		}
	}

	/* HARD MODE
	// Check side for peripheral vision.
	for (int i = 0; i < m_maxViewDist - 2; ++i)
	{
		glm::vec2 endPos{ guardWorldPos };
		endPos.x -= ((m_tileViewRadius + 1) * LevelScene::TILE_SIZE);
		endPos.y += ((i + 1) * LevelScene::TILE_SIZE);
		m_isPlayerDetected |= tryDetectPlayer(guardWorldPos, endPos);
	}

	// Check the other side for peripheral vision.
	for (int i = 0; i < m_maxViewDist - 2; ++i)
	{
		glm::vec2 endPos{ guardWorldPos };
		endPos.x += ((m_tileViewRadius + 1) * LevelScene::TILE_SIZE);
		endPos.y += ((i + 1) * LevelScene::TILE_SIZE);
		m_isPlayerDetected |= tryDetectPlayer(guardWorldPos, endPos);
	}
	*/

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
		GameObject* text = new GameObject();
		text->AddBehaviour(new TextBehaviour("Lose!", 2, glm::vec3(1, 0, 0)));
		m_levelScene->AddScreenGameObject(text);
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

bool GuardDetection::tryDetectPlayer(glm::vec2 startPoint, glm::vec2 maxDistancePoint)
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
			return true;
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
