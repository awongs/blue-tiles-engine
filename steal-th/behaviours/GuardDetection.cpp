#include "GuardDetection.h"
#include "../scenes/LevelScene.h"
#include "../gameobjects/Tile.h"

#include <engine/debugbt/DebugLog.h>
#include <engine/GameObject.h>
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

	 // Returns true if using the "low" version, otherwise returns false 
	 // if using the "high" version. This is important because the order
	 // of points is reversed depending on which version we use.
	 bool PlotLine(int x0, int y0, int x1, int y1, std::vector<glm::ivec2> &output) {
		 if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
			 if (x0 > x1) {
				 PlotLineLow(x1, y1, x0, y0, output);
			 }
			 else {
				 PlotLineLow(x0, y0, x1, y1, output);
			 }

			 return true;
		 }
		 else {
			 if (y0 > y1) {
				 PlotLineHigh(x1, y1, x0, y1, output);
			 }
			 else {
				 PlotLineHigh(x0, y0, x1, y1, output);
			 }

			 return false;
		 }
	 }
}

GuardDetection::GuardDetection(LevelScene *levelScene, GameObject *playerObj,
	float maxViewDist, int tileViewRadius) :
	Behaviour(BehaviourType::GuardDetection),
	m_levelScene(levelScene), m_playerObj(playerObj), 
	m_maxViewDist(maxViewDist), m_tileViewRadius(tileViewRadius)
{
    m_playerDetected = false;
}

void GuardDetection::Update(float deltaTime)
{
	// Reset the detected flag.
	m_playerDetected = false;

	// Don't bother continuing if we can't even get the tile data.
	if (m_levelScene == nullptr)
		return;

	// The guard's tile position is point 1 for the line algorithm.
	// We want this to be the tile directly in front of the guard.
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };
	glm::vec2 frontWorldPos{ guardWorldPos.x, guardWorldPos.y - LevelScene::TILE_SIZE };

	// For rotation == 0.f, the guard is facing up.
	glm::vec2 unrotatedMaxDistPoint{ frontWorldPos.x, frontWorldPos.y - m_maxViewDist };

	// Check if the guard can detect the player at its maximum view distance.
	int numEndTiles{ m_tileViewRadius * 2 };
	for (int i = 0; i < numEndTiles + 1; ++i)
	{
		glm::vec2 pos{ unrotatedMaxDistPoint };
		pos.x -= (m_tileViewRadius * LevelScene::TILE_SIZE);
		pos.x += (i * LevelScene::TILE_SIZE);
		tryDetectPlayer(frontWorldPos, pos);
	}

	// Check side for peripheral vision.
	for (int i = 0; i < m_tileViewRadius; ++i)
	{
		glm::vec2 startPos{ frontWorldPos };
		startPos.x -= (2 * LevelScene::TILE_SIZE);

		glm::vec2 pos{ startPos };
		pos.x -= (m_maxViewDist - 2 * LevelScene::TILE_SIZE);
		pos.y -= (i * LevelScene::TILE_SIZE);
		tryDetectPlayer(startPos, pos);
	}

	// Check the other side for peripheral vision.
	for (int i = 0; i < m_tileViewRadius; ++i)
	{
		glm::vec2 startPos{ frontWorldPos };
		startPos.x += (2 * LevelScene::TILE_SIZE);

		glm::vec2 pos{ startPos };
		pos.x += (m_maxViewDist - 2 * LevelScene::TILE_SIZE);
		pos.y -= (i * LevelScene::TILE_SIZE);
		tryDetectPlayer(startPos, pos);
	}

	// Additional peripheral vision checks.
	// We need to explicitly check the two tiles beside the front tile,
	// since we skip it above.
	glm::vec2 pos{ frontWorldPos.x - LevelScene::TILE_SIZE, frontWorldPos.y };
	tryDetectPlayer(frontWorldPos, pos);
	pos.x = frontWorldPos.x + LevelScene::TILE_SIZE;
	tryDetectPlayer(frontWorldPos, pos);

	// Check the tile on the guard, as well as the two tiles
	// to the side of the guard.
	glm::vec2 endPos{ guardWorldPos };
	endPos.x -= LevelScene::TILE_SIZE;
	tryDetectPlayer(guardWorldPos, endPos);
	endPos = guardWorldPos;
	endPos.x += LevelScene::TILE_SIZE;
	tryDetectPlayer(guardWorldPos, endPos);
}

void GuardDetection::Draw(Shader& shader) {}

bool GuardDetection::HandleMessage(unsigned int senderID, std::string& message) {
    return false;
}

void GuardDetection::OnCollisionStay(GLuint other)
{
}

void GuardDetection::tryDetectPlayer(glm::vec2 startPoint, glm::vec2 maxDistancePoint)
{
	// Rotate the vector (start point to max distance point) by
	// the guard's rotation.
	glm::vec2 viewVector{ maxDistancePoint - startPoint };
	viewVector = glm::rotate(viewVector, gameObject->rotation.y);

	// Rotate the vector (guard's position to start point) by
	// the guard's rotation.
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };
	glm::vec2 startVector{ startPoint - guardWorldPos };
	startVector = glm::rotate(startVector, gameObject->rotation.y);

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
		return;
	}

	// Call the line algorithm to get the points on the line.
	std::vector<glm::ivec2> output;
	bool isUsingLow{ PlotLine(guardTilePos.x, guardTilePos.y, maxTileDistPoint.x, maxTileDistPoint.y, output) };

	glm::vec2 playerWorldPos{ glm::vec2(m_playerObj->position.x, m_playerObj->position.z) };
	glm::ivec2 playerTilePos{ m_levelScene->GetTileCoordFromPos(playerWorldPos) };

	// Check points on the line, ordered from closest point to the 
	// guard to furthest.
	// The output of the line algorithm has its points in an order
	// that depends on whether we use the "low" or "high" version.
	// Iterate through the output vector in reverse order if
	// using the "low" version.
	auto currentIt{ isUsingLow ? output.end() - 1 : output.begin() };
	auto endIt{ isUsingLow ? output.begin() : output.end() };
	while (currentIt != endIt)
	{
		glm::ivec2 point{ *currentIt };
		//DebugLog::Info(std::to_string(point.x) + ", " + std::to_string(point.y));

		if (point == playerTilePos)
		{
			m_playerDetected = true;
			DebugLog::Info("I GOT U IN MY SIGHTS");
			break;
		}

		// Check if the vision ray has hit a wall yet.
		TileType tile{ m_levelScene->GetTile(point.x, point.y) };
		if (tile == TileType::WALL ||
			tile == TileType::RED_DOOR ||
			tile == TileType::GREEN_DOOR ||
			tile == TileType::BLUE_DOOR)
		{
			break;
		}

		currentIt += (isUsingLow ? -1 : 1);
	}
}
