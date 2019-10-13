#include "GuardDetection.h"
#include "../scenes/LevelScene.h"
#include "../gameobjects/Tile.h"

#include <engine/debugbt/DebugLog.h>
#include <engine/GameObject.h>
#include <vector>
#include <cmath>
#include <glm/gtx/rotate_vector.hpp>

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

	 void PlotLine(int x0, int y0, int x1, int y1, std::vector<glm::ivec2> &output) {
        if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
            if(x0 > x1) {
				PlotLineLow(x1, y1, x0, y0, output);
            }else {
				PlotLineLow(x0, y0, x1, y1, output);
            }
        } else {
            if(y0 > y1) {
				PlotLineHigh(x1, y1, x0, y1, output);
            }else {
				PlotLineHigh(x0, y0, x1, y1, output);
            }
        }
    }
}

GuardDetection::GuardDetection(LevelScene *levelScene, GameObject *playerObj) :
	Behaviour(BehaviourType::GuardDetection),
	m_levelScene(levelScene), m_playerObj(playerObj)
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
	glm::vec2 guardWorldPos{ gameObject->position.x, gameObject->position.z };
	glm::ivec2 guardTilePos{ m_levelScene->GetTileCoordFromPos(guardWorldPos) };

	// TODO: change this fixed view distance.
	float maxViewDist{ LevelScene::TILE_SIZE * 3 };

	// For rotation == 0.f, the guard is facing up.
	glm::vec2 unrotatedMaxDistPoint{ guardWorldPos.x, guardWorldPos.y - maxViewDist };

	glm::vec2 viewVector{ unrotatedMaxDistPoint - guardWorldPos };
	viewVector = glm::rotate(viewVector, gameObject->rotation.y);

	// The max tile distance point is point 2 for the line algorithm.
	glm::vec2 maxWorldDistPoint{ guardWorldPos + viewVector };
	glm::ivec2 maxTileDistPoint{ m_levelScene->GetTileCoordFromPos(maxWorldDistPoint) };
	glm::ivec2 levelSize{ m_levelScene->GetLevelSize() };
	maxTileDistPoint.x = glm::clamp(maxTileDistPoint.x, 0, levelSize.x - 1);
	maxTileDistPoint.y = glm::clamp(maxTileDistPoint.y, 0, levelSize.y - 1);

	// Call the line algorithm to get the points on the line.
	std::vector<glm::ivec2> output;
	PlotLine(guardTilePos.x, guardTilePos.y, maxTileDistPoint.x, maxTileDistPoint.y, output);

	glm::vec2 playerWorldPos{ glm::vec2(m_playerObj->position.x, m_playerObj->position.z) };
	glm::ivec2 playerTilePos{ m_levelScene->GetTileCoordFromPos(playerWorldPos) };

	// Check points on the line, ordered from closest point to the 
	// guard to furthest.
	// The output of the line algorithm has this in reverse order, 
	// so we iterate through the output vector in reverse.
	for (auto it = output.rbegin(); it != output.rend(); ++it)
	{
		glm::ivec2 point{ *it };
		//DebugLog::Info(std::to_string(point.x) + ", " + std::to_string(point.y));

		if (point == playerTilePos)
		{
			m_playerDetected = true;
			DebugLog::Info("I GOT U IN MY SIGHTS");
			break;
		}

		// TODO: fix this... otherwise we're considering any tile with a 
		// wall/door in it to be obstructing vision, regardless of the 
		// facing-direction of that wall/door.

		// Check if the vision ray has hit a wall yet.
		const Tile &tile{ m_levelScene->GetTile(point.x, point.y) };
		bool isBlocked{
			tile.up == Tile::TileEdgeType::WALL ||
			tile.up == Tile::TileEdgeType::DOOR ||
			tile.down == Tile::TileEdgeType::WALL ||
			tile.down == Tile::TileEdgeType::DOOR ||
			tile.left == Tile::TileEdgeType::WALL ||
			tile.left == Tile::TileEdgeType::DOOR ||
			tile.right == Tile::TileEdgeType::WALL ||
			tile.right == Tile::TileEdgeType::DOOR 
		};

		// This vision ray has hit a wall.
		if (isBlocked)
			break;
	}
}

void GuardDetection::Draw(Shader& shader) {}

bool GuardDetection::HandleMessage(unsigned int senderID, std::string message) {
    return false;
}