#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <unordered_map>
#include <iostream>
#include "Box2DTiledLoader.h"
#include "TileLayer.h"
#include "ObjectGroup.h"

Box2DTiledLoader::Box2DTiledLoader() : mWorldLoaded(false), mMapWidth(0), mMapHeight(0){}

void Box2DTiledLoader::load(const tiled::TileGrid& grid){
    
    mWorldLoaded = false;
    float gravityVal = -1 * ((2 * MAX_JUMP_HEIGHT) / powf(JUMP_APEX_TIME,2.f));
    b2Vec2 gravity(0.0f, gravityVal);
    mWorld = new b2World(gravity);
    mMapHeight = grid.size();
    mMapWidth = grid[0].size();

    BitGrid visitedGrid;
    for (int row = 0; row < mMapHeight; ++row){
        BitVector currentRow;
        for (int col = 0; col < mMapWidth; ++col){
            currentRow.push_back(false);
        }
        visitedGrid.push_back(currentRow);
    }

    bool tracing = true;

    while (tracing){
        tracing = contourTrace(grid, visitedGrid);
    }
    std::cout << "All contours have been traced!" << std::endl;
    mWorldLoaded = true;
}

bool Box2DTiledLoader::isWorldLoaded(){ return mWorldLoaded; }

b2World* Box2DTiledLoader::getWorld() const{ return mWorld; }

std::vector<b2Body*> Box2DTiledLoader::getContours() const{ return mContours; }

bool Box2DTiledLoader::contourTrace(const tiled::TileGrid &grid, BitGrid &visited){

    //Find the first tile cell with an adjacent blank cell
    for (int row = 0; row < mMapHeight; ++row){
        for (int col = 0; col < mMapWidth; ++col){
            if (!visited[row][col] && grid[row][col].GID){
                if ((row > 0) && (!grid[row-1][col].GID)){
                    contourTraceComponent(col, row, col, row-1, grid, visited);
                    return true;
                }
                else if ((row < mMapHeight-1) && (!grid[row+1][col].GID)){
                    contourTraceComponent(col, row, col, row+1, grid, visited);
                    return true;
                }
                else if ((col > 0) && (!grid[row][col-1].GID)){
                    contourTraceComponent(col, row, col-1, row, grid, visited);
                    return true;
                }
                else if ((col < mMapWidth-1) && (!grid[row][col+1].GID)){
                    contourTraceComponent(col, row, col+1, row, grid, visited);
                    return true;
                }
            }
        }       
    }
    return false;
}

void Box2DTiledLoader::contourTraceComponent(int startX, int startY, int startBacktrackX, int startBacktrackY, const tiled::TileGrid &grid, BitGrid &visited){
    
    visited[startY][startX] = true;
    std::vector<b2Vec2> chainVertices;

    //Set up our initial chain edge    
    auto neighborhoodCoords = getMooreNeighborhood(startX, startY);
    int i = getMooreIndex(startX, startY, startBacktrackX, startBacktrackY);
    int cClockwiseIndex = (i == 0 ? 6 : i -2);
    addChainVertex(
        startX,
        startY, 
        neighborhoodCoords[cClockwiseIndex].first,
        neighborhoodCoords[cClockwiseIndex].second, 
        chainVertices);
    addChainVertex(startX, 
        startY, 
        neighborhoodCoords[i].first, 
        neighborhoodCoords[i].second, chainVertices);

    //Begin Moore-Neighbor Tracing
    int boundaryX = startX;
    int boundaryY = startY;
    int currentX = startBacktrackX;
    int currentY = startBacktrackY;
    int backtrackX = -1;
    int backtrackY = -1;
    int numStartVisits = 0;

    //Using Jacob's stopping criterion along with 
    //the start cell being visited a maximum of 3 times
    while ((!((boundaryX == startX) && 
              (boundaryY == startY) && 
              (backtrackX == startBacktrackX) && 
              (backtrackY == startBacktrackY))) &&
              (numStartVisits < 3)){

        if (boundaryX == startX && boundaryY == startY){
            if (numStartVisits > 0){
                startBacktrackX = backtrackX;
                startBacktrackY = backtrackY;
            }
            ++numStartVisits;
        }

        auto neighborhoodCoords = getMooreNeighborhood(boundaryX, boundaryY);
        int i = getMooreIndex(boundaryX, boundaryY, currentX, currentY);
        int numVisited = 0;
        auto invalidPair = std::make_pair(-1, -1);

        while (!grid[currentY][currentX].GID){

            backtrackX = currentX;
            backtrackY = currentY;
            currentX = neighborhoodCoords[i].first;
            currentY = neighborhoodCoords[i].second;           
            ++numVisited;

            //Handle case when the map is not closed
            //I.E. we go off the map contour tracing
            if (neighborhoodCoords[i] == invalidPair){
                createStaticBody(chainVertices);
                return;
            }     

            i = (i == 7 ? 0 : i + 1);

            //Handle single tile
            if (numVisited == 9){
                createStaticBody(chainVertices);
                return;
            }

            //Handle edges not adjacent to other tiles
            if ((!grid[currentY][currentX].GID) && (i % 2 == 1) && (numVisited > 2)){
                b2Vec2 edgeV1, edgeV2;
                addChainVertex(
                    boundaryX, 
                    boundaryY, 
                    neighborhoodCoords[i-1].first, 
                    neighborhoodCoords[i-1].second, 
                    chainVertices);
            }
        }
        visited[currentY][currentX] = true;
        boundaryX = currentX;
        boundaryY = currentY;
        currentX = backtrackX;
        currentY = backtrackY;
        addChainVertex(boundaryX, boundaryY, backtrackX, backtrackY, chainVertices);
    }
    createStaticBody(chainVertices);
}

void Box2DTiledLoader::addChainVertex(int startX, int startY, int previousX, int previousY, std::vector<b2Vec2> &chainVertices){

    b2Vec2 v1;

    //Visited from below
    if (startY - previousY == -1){
        v1.Set(
            static_cast<float>(startX),
            static_cast<float>(mMapHeight-(startY+1)));
    }
    //Visited from above
    else if (startY - previousY == 1){
        v1.Set(
            static_cast<float>(startX+1),
            static_cast<float>(mMapHeight-(startY)));
    }
    //Visited from the left
    else if (startX - previousX == 1){
        v1.Set(
            static_cast<float>(startX),
            static_cast<float>(mMapHeight-(startY)));
    }
    //Visited from the right
    else{
        v1.Set(
            static_cast<float>(startX+1),
            static_cast<float>(mMapHeight-(startY+1)));
    }
    chainVertices.push_back(v1);
}

void Box2DTiledLoader::createStaticBody(std::vector<b2Vec2>& chainVertices){

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.allowSleep = true;
    bodyDef.awake = true;
    b2Body *platform = mWorld->CreateBody(&bodyDef);

    //Remove duplicate edges from contour trace so Box2D doesn't yell at us
    auto chainLoop = std::find(chainVertices.begin()+1, chainVertices.end(), chainVertices[0]);
    if (chainLoop != chainVertices.end())
        chainVertices.erase(chainLoop+1, chainVertices.end());
    int numVertices = chainVertices.size();

    //Attach our fixtures 8 vertices at a time
    b2Vec2 vertices[8];
    vertices[0] = chainVertices[0];

    for (int i = 0; i < numVertices/7; ++i){
        if (i*8 >= numVertices)
            continue;
        for (int j = 1; j < 8; ++ j){
            vertices[j] = chainVertices[i*8 + (j-i)];
        }
        b2ChainShape chain;
        chain.CreateChain(vertices,8);
        b2Fixture *contourFixture = platform->CreateFixture(&chain,0);
        contourFixture ->SetFriction(1.f);

        //Make sure all edges are connected
        vertices[0] = vertices[7];
    }

    //Have to construct our last chain separately since 
    //it may be shorter than our predetermined length
    int lastChainLength = numVertices % 7;

    if (numVertices < 8){
        for (int i = 0; i < numVertices; ++i)
            vertices[i] = chainVertices[i];
    }
    else{
        for (int i = 1; i < lastChainLength; ++i){
            int index = numVertices - lastChainLength + i;
            vertices[i] = chainVertices[index];
        }
    }
    if (lastChainLength >= 2){
        b2ChainShape lastChain;
        lastChain.CreateChain(vertices,lastChainLength);
        b2Fixture *lastContourFixture = platform->CreateFixture(&lastChain,0);
    }
    mContours.push_back(platform);
}

std::vector<std::pair<int,int>> Box2DTiledLoader::getMooreNeighborhood(int startX, int startY){

    std::vector<std::pair<int,int>> neighborhood;

    //Start from the left-tile and go clockwise
    //Values of -1 indicate border cases where the cell is outside the grid
    bool exists = (startX > 0);
    neighborhood.push_back(
        std::make_pair(exists ? startX-1 : -1, exists ? startY : -1));

    exists = (startX > 0 && startY > 0);
    neighborhood.push_back(
        std::make_pair(exists ? startX-1 : -1, exists ? startY-1 : -1));

    exists = (startY > 0);
    neighborhood.push_back(
        std::make_pair(exists ? startX : -1, exists ? startY-1 : -1));

    exists = (startX < mMapWidth-1 && startY > 0);
    neighborhood.push_back(
        std::make_pair(exists ? startX+1 : -1, exists ? startY-1 : -1));

    exists = (startX < mMapWidth-1);
    neighborhood.push_back(
        std::make_pair(exists ? startX+1 : -1, exists ? startY : -1));

    exists = (startX < mMapWidth-1 && startY < mMapHeight-1);
    neighborhood.push_back(
        std::make_pair(exists ? startX+1 : -1, exists ? startY+1 : -1));

    exists = (startY < mMapHeight-1);
    neighborhood.push_back(
        std::make_pair(exists ? startX : -1, exists ? startY+1 : -1));

    exists = (startX > 0 && startY < mMapHeight-1);
    neighborhood.push_back(
        std::make_pair(exists ? startX-1 : -1, exists ? startY+1 : -1));
    return neighborhood;
}

int Box2DTiledLoader::getMooreIndex(int boundaryX, int boundaryY, int neighborX, int neighborY){

    if ((neighborX == boundaryX - 1) && (neighborY == boundaryY))
        return 0;
    if ((neighborX == boundaryX - 1) && (neighborY == boundaryY -1))
        return 1;
    if ((neighborX == boundaryX) && (neighborY == boundaryY -1))
        return 2;
    if ((neighborX == boundaryX + 1) && (neighborY == boundaryY - 1))
        return 3;
    if ((neighborX == boundaryX + 1) && (neighborY == boundaryY))
        return 4;
    if ((neighborX == boundaryX +1) && (neighborY == boundaryY +1))
        return 5;
    if ((neighborX == boundaryX) && (neighborY == boundaryY +1))
        return 6;
    if ((neighborX == boundaryX -1) && (neighborY == boundaryY +1))
        return 7;
    return -1;
}