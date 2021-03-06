#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
#include <queue>

class SceneNode;

namespace Category{
    enum Type{
        None = 0, //Unclassified
        Walkable = 1 << 0, //Platforms, tops of boxes, etc
        Player = 1 << 1, //Player
        Enemy = 1 << 2, //Enemy
        GrassBlock = 1 << 3, //GrassBlock entity
        GrassPlatform = 1 << 4, //GrassPlatform entity
        Seeker = 1 << 5, //For following player
        Gem = 1 << 6, //Level gem
        Exit = 1 << 7, //End of level
        Damager = 1 << 8, //Spikes, lava, etc
        SoundEffect = 1 << 9 //Effects
    };
}

struct Command{

    std::function<void(SceneNode&, sf::Time)> action;
    unsigned int category;  
};

typedef std::queue<Command> CommandQueue;