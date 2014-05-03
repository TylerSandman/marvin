#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace tiled{

    struct Tile{
    public:
        Tile(): GID(0){}
    public:
        unsigned int GID;
        sf::Sprite sprite;
    };

    typedef std::vector<Tile> Tiles;
    typedef std::vector<Tiles> TileGrid;
}