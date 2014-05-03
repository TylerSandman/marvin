#pragma once
#include <Box2D\Box2D.h>
#include "TileLayer.h"
#include "ObjectGroup.h"

//These keep track of which tile positions are visited by our contour tracing
typedef std::vector<bool> BitVector;
typedef std::vector<BitVector> BitGrid;

const float MIN_JUMP_HEIGHT = 1.f;
const float MAX_JUMP_HEIGHT = 5.f;
const float JUMP_APEX_TIME = 0.5f;
const float ACCELERATION_CONSTANT = 0.35f;
const float DECELERATION_CONSTANT = 0.95f;
const float MAX_RUN_SPEED = 10.f;

class Box2DTiledLoader{

public:
    Box2DTiledLoader();
    void load(const tiled::TileGrid& grid);
    bool isWorldLoaded();
    b2World* getWorld();

private:
    bool contourTrace(const tiled::TileGrid &grid, BitGrid &visited);
    void contourTraceComponent(int startX, int startY, int startBacktrackX, int startBacktrackY, const tiled::TileGrid &grid, BitGrid &visited);
    void addChainVertex(int startX, int startY, int previousX, int previousY, std::vector<b2Vec2> &chainVertices);
    void createStaticBody(std::vector<b2Vec2>& chainVertices);
    std::vector<std::pair<int,int>> getMooreNeighborhood(int startX, int startY);
    int getMooreIndex(int boundaryX, int boundaryY, int neighborX, int neighborY);

private:
    b2World * mWorld;
    bool mWorldLoaded;
    int mMapWidth;
    int mMapHeight;
};
