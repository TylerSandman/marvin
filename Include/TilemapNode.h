#pragma once
#include <SFML\Graphics.hpp>
#include "SceneNode.h"
#include "MapData.h"

class TilemapNode: public SceneNode{

public:
    explicit TilemapNode(const MapData& mapData);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    MapData mMapData;
};

