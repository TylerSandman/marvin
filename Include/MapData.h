#pragma once
#include "TileLayer.h"
#include "ObjectGroup.h"
struct MapData{

    //All in pixels
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    tiled::TileLayers tileLayers;
    tiled::ObjectGroups objectGroups;
};