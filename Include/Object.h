#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace tiled{

    struct Object{
    public:
        Object(): GID(0){}
    public:
        unsigned int GID;
        bool visible;
        std::string name;
        std::string type;
        sf::Vector2f position;
        sf::Sprite sprite;
    };

    typedef std::vector<Object> Objects;
};