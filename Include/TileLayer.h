#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tile.h"

namespace tiled{

    struct TileLayer{

    public:
        tiled::TileGrid tiles;
        std::string name;
        bool visible;
        double opacity;
    };

    typedef std::vector<TileLayer> TileLayers;
}