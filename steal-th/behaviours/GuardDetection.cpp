#include "GuardDetection.h"
#include <engine/debugbt/DebugLog.h>
#include <vector>
#include <cmath>
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

    std::vector<glm::vec2> PlotLineLow(int & x0, int & y0, int & x1, int & y1) {
        //Vector containing points on the line.
        std::vector<glm::vec2 *> searchTiles;

        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1;

        if(dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int D = 2 * dy - dx;
        int y = y0;

        for(int x = x0; i < x1; x++) {
            searchTiles.push_back(new vec2(x,y));
            
            if(D > 0 ) {
                y = y + yi;
                D = D - 2 * dx;
            }
            
            D = D + 2 * dy;
        }

        return searchTiles;
    }

     std::vector<glm::vec2> PlotLineHigh(int & x0, int & y0, int & x1, int & y1) {
        //Vector containing points on the line.
        std::vector<glm::vec2 *> searchTiles;

        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = 1;

        if(dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int D = 2 * dx - dy;
        int x = x0;

        for(int y = y0; i < y1; y++) {
            searchTiles.push_back(new vec2(x,y));
            
            if(D > 0 ) {
                x = x + xi;
                D = D - 2 * dy;
            }
            
            D = D + 2 * dx;
        }

        return searchTiles;
    }

    std::vector<glm::vec2> PlotLine(int & x0, int & y0, int & x1, int & y1) {
        std::vector<glm::vec2 *> searchTiles;
        if(std::abs(y1 - y0) < std::abs(x1 - x0)) {
            if(x0 > x1) {
                searchTiles = PlotLineLow(x1, y1, x0, y0);
            }else {
                searchTiles = PlotLineLow(x0, y0, x1, y1);
            }
        }else {
            if(y0 > y1) {
                searchTiles = PlotLineHigh(x1, y1, x0, y1);
            }else {
                searchTiles = PlotLineHigh(x0, y0, x1, y1);
            }
        }
        return searchTiles;
    }
}