#pragma once
#include "TileLayer.h"
#include "ObjectGroup.h"
struct MapData{

    //In tiles
    int mapWidth;
    int mapHeight;

    //In Pixels
    int tileWidth;
    int tileHeight;

    tiled::TileLayers tileLayers;
    tiled::ObjectGroups objectGroups;
};