float2 rotateVec2(float2 input, float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);

	float resultX = input.x * cosAngle - input.y * sinAngle;
	float resultY = input.x * sinAngle + input.y * cosAngle;
	return (float2)(resultX, resultY);
}

int2 getTileCoordFromPos(float2 pos, float tileSize)
{
	return (int2)(floor(pos.x / tileSize), floor(pos.y / tileSize));
}

int getTile(__global const int* levelTiles, int levelX, int levelZ, int2 tilePos)
{
	int clampedX = clamp(tilePos.x, 0, levelX - 1);
	int clampedZ = clamp(tilePos.y, 0, levelZ - 1);
	int index = clampedZ * levelX + clampedX;

	return levelTiles[index];
}

void PlotLineLow(int x0, int y0, int x1, int y1, __global int2* output, int* outputSize) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;

	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y0;

	int i = 0;
	for (int x = x0; x <= x1; x++) {
		output[i] = (int2)(x, y);
		i++;

		if (D > 0) {
			y = y + yi;
			D = D - 2 * dx;
		}

		D = D + 2 * dy;
	}

	*outputSize = i;
}

void PlotLineHigh(int x0, int y0, int x1, int y1, __global int2* output, int* outputSize) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;

	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x0;

	int i = 0;
	for (int y = y0; y <= y1; y++) {
		output[i] = (int2)(x, y);
		i++;

		if (D > 0) {
			x = x + xi;
			D = D - 2 * dy;
		}

		D = D + 2 * dx;
	}

	*outputSize = i;
}

// Returns true if input coordinates were reversed, otherwise returns false.
bool PlotLine(int x0, int y0, int x1, int y1, __global int2* output, int* outputSize) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			PlotLineLow(x1, y1, x0, y0, output, outputSize);
			return true;
		}
		else {
			PlotLineLow(x0, y0, x1, y1, output, outputSize);
			return false;
		}
	}
	else {
		if (y0 > y1) {
			PlotLineHigh(x1, y1, x0, y0, output, outputSize);
			return true;
		}
		else {
			PlotLineHigh(x0, y0, x1, y1, output, outputSize);
			return false;
		}
	}
}

__kernel void guard_detection(__global const float* endPointsX,
    __global const float* endPointsZ,
	__global bool* output,
	const float startPointX,
	const float startPointZ,
	const float rotation,
	const float tileSize,
	const int levelX,
	const int levelZ,
	__global const int* levelTiles,
	const float playerPosX,
	const float playerPosZ,
    __global int2* lineAlgOutput)
{
    int gid = get_global_id(0);

	// Rotate the vector (start point to max distance point) by
	// the guard's rotation.
	float2 endPoint = (float2)(endPointsX[gid], endPointsZ[gid]);
	float2 startPoint = (float2)(startPointX, startPointZ);
	float2 viewVector = endPoint - startPoint;
	viewVector = rotateVec2(viewVector, -rotation);

	// The max tile distance point is point 2 for the line algorithm.
	float2 maxWorldDistPoint = startPoint + viewVector;
	int2 maxTileDistPoint = getTileCoordFromPos(maxWorldDistPoint, tileSize);
	maxTileDistPoint.x = clamp(maxTileDistPoint.x, 0, levelX - 1);
	maxTileDistPoint.y = clamp(maxTileDistPoint.y, 0, levelZ - 1);

	// If the starting point is outside of the level, then just cancel this
	// detection check.
	int2 guardTilePos = getTileCoordFromPos(startPoint, tileSize);

    if (guardTilePos.x < 0 || guardTilePos.x > levelX - 1 ||
    	guardTilePos.y < 0 || guardTilePos.y > levelZ - 1)
    {
        output[gid] = false;
    	return;
    }

    // Call the line algorithm to get the points on the line.
    int size = (int)distance(convert_float2(guardTilePos), convert_float2(maxTileDistPoint));
    int outputSize = 0;
    bool isUsingLow = PlotLine(guardTilePos.x, guardTilePos.y, maxTileDistPoint.x, maxTileDistPoint.y, lineAlgOutput, &outputSize);

    printf("Test from kernel\n");
    float2 playerWorldPos = (float2)(playerPosX, playerPosZ);
    int2 playerTilePos = getTileCoordFromPos(playerWorldPos, tileSize);

    // Check points on the line, ordered from closest point to the
    // guard to furthest.
    // The output of the line algorithm has its points in an order
    // that depends on whether the input coordinates were reversed.
    __global int2* currentIt = isUsingLow ? lineAlgOutput + outputSize - 1 : lineAlgOutput;
    int currentCount = 0;

    // Hold the values corresponding to each tile type.
    const int WALL = 1;
    const int RED_DOOR = 2;
    const int GREEN_DOOR = 3;
    const int BLUE_DOOR = 4;

    // Hold the output value, which is a flag of whether the player
    // was detected.
    bool result = false;

    while (true)
    {
    	int2 point = *currentIt;

    	// Check if the vision ray has hit a wall yet.
    	int tile = getTile(levelTiles, levelX, levelZ, point);
    	if (tile == WALL ||
    		tile == RED_DOOR ||
    		tile == GREEN_DOOR ||
    		tile == BLUE_DOOR)
    	{
    		result = false;
    		break;
    	}

    	// Check if the player is on this tile.
    	if (point.x == playerTilePos.x && point.y == playerTilePos.y)
    	{
    		result = true;
    		break;
    	}

    	// Check adjacent walls to handle corner cases, when looking
    	// diagonally through this tile.
    	if (currentCount + 1 < outputSize)
    	{
    		int2 nextPoint = *(currentIt + (isUsingLow ? -1 : 1));
    		int2 diff = nextPoint - point;

    		int cornerTile = 0;
    		int cornerTile2 = 0;

    		// Next point is moving toward top-left.
    		if (diff.x < 0 && diff.y < 0)
    		{
    			cornerTile = getTile(levelTiles, levelX, levelZ, (int2)(point.x - 1, point.y));
    			cornerTile2 = getTile(levelTiles, levelX, levelZ, (int2)(point.x, point.y - 1));
    		}
    		// Next point is moving toward top-right.
    		else if (diff.x > 0 && diff.y < 0)
    		{
    			cornerTile = getTile(levelTiles, levelX, levelZ, (int2)(point.x + 1, point.y));
    			cornerTile2 = getTile(levelTiles, levelX, levelZ, (int2)(point.x, point.y - 1));
    		}
    		// Next point is moving toward bottom-left.
    		else if (diff.x < 0 && diff.y > 0)
    		{
    			cornerTile = getTile(levelTiles, levelX, levelZ, (int2)(point.x - 1, point.y));
    			cornerTile2 = getTile(levelTiles, levelX, levelZ, (int2)(point.x, point.y + 1));
    		}
    		// Next point is moving toward bottom-right.
    		else if (diff.x > 0 && diff.y > 0)
    		{
    			cornerTile = getTile(levelTiles, levelX, levelZ, (int2)(point.x + 1, point.y));
    			cornerTile2 = getTile(levelTiles, levelX, levelZ, (int2)(point.x, point.y + 1));
    		}

    		bool isBlockingCorner1 = (cornerTile == WALL ||
    			cornerTile == RED_DOOR ||
    			cornerTile == GREEN_DOOR ||
    			cornerTile == BLUE_DOOR);

    		bool isBlockingCorner2 = (cornerTile2 == WALL ||
    			cornerTile2 == RED_DOOR ||
    			cornerTile2 == GREEN_DOOR ||
    			cornerTile2 == BLUE_DOOR);

    		if (isBlockingCorner1 || isBlockingCorner2)
    		{
    			result = false;
    			break;
    		}
    	}

    	// Done checking all points.
    	if (++currentCount == outputSize)
    	{
    		result = false;
    		break;
    	}

    	// Otherwise, move the iterator to the next point.
    	currentIt += (isUsingLow ? -1 : 1);
    }

    // Set the output.
    output[gid] = result;
}
