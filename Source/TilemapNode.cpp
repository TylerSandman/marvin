#include <SFML/Graphics.hpp>
#include "World.h"
#include "TilemapNode.h"
#include "Command.h"

TilemapNode::TilemapNode(const MapData &mapData, std::vector<b2Body*> contours) :
        mMapData(mapData){

    //Make the contours aware of the node for collision handling
    for(auto &contour : contours){
        contour->SetUserData(this);
    }
}

unsigned int TilemapNode::getCategory() const{
    return Category::Type::Walkable;
}

void TilemapNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{

    //Culling logic to only render visible tiles
    sf::Vector2f center = target.getView().getCenter();
    sf::Vector2f size = target.getView().getSize();
    int viewStartCullX = static_cast<int>
        (std::floor((center.x - (size.x/2))/mMapData.tileWidth)) - 1;
    int viewStartCullY = static_cast<int>
        (std::floor((center.y - (size.y/2))/mMapData.tileHeight)) - 1;
    int viewEndCullX = static_cast<int>
        (std::ceil((center.x + (size.x/2))/mMapData.tileWidth)) + 1;
    int viewEndCullY = static_cast<int>
        (std::ceil((center.y + (size.y/2))/mMapData.tileHeight)) + 1;

    if (viewStartCullX < 0) 
        viewStartCullX = 0;
    if (viewStartCullX > mMapData.mapWidth)
        viewStartCullX = mMapData.mapWidth;
    if (viewStartCullY < 0)
        viewStartCullY = 0;
    if (viewStartCullY > mMapData.mapHeight)
        viewStartCullY = mMapData.mapHeight;
    if (viewEndCullX < 0) 
        viewEndCullX = 0;
    if (viewEndCullX > mMapData.mapWidth)
        viewEndCullX = mMapData.mapWidth;
    if (viewEndCullY < 0)
        viewEndCullY = 0;
    if (viewEndCullY > mMapData.mapHeight)
        viewEndCullY = mMapData.mapHeight;

    for (int i = viewStartCullY; i < viewEndCullY; ++i){
        for (int j = viewStartCullX; j < viewEndCullX; ++j){
            for(auto &layer : mMapData.tileLayers){
                auto tile = layer.tiles[i][j];
                if (tile.sprite.getTexture()) 
                    target.draw(tile.sprite);
            }
        }
    }
}