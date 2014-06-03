#pragma once
#include <SFML/Graphics.hpp>
#include <json_spirit_reader_template.h>
#include <vector>
#include <map>
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
        std::map<std::string, json_spirit::mValue> properties;
        sf::Vector2f position;

        sf::Sprite sprite;
    };

    typedef std::vector<Object> Objects;
};