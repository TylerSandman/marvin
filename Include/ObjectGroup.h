#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Object.h"

namespace tiled{

    struct ObjectGroup{

    public:
        tiled::Objects objects;
        std::string name;
        bool visible;
        double opacity;
    };

    typedef std::vector<ObjectGroup> ObjectGroups;
};