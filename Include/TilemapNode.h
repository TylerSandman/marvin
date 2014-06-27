#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include "SceneNode.h"
#include "MapData.h"

class TilemapNode: public SceneNode{

public:
    TilemapNode(const MapData& mapData, std::vector<b2Body*> contours);
    unsigned int getCategory() const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    MapData mMapData;
    std::vector<b2Body*> mContours;
};

