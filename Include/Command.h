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
        Pickup = 1 << 3, //Coins, powerups, etc
        Interactable = 1 << 4, //Coin blocks, springs, etc
        Damager = 1 << 5, //Spikes, lava, etc
        Projectile = 1 << 6 //Bullet, fireball ,etc
    };
}

struct Command{

    std::function<void(SceneNode&, sf::Time)> action;
    unsigned int category;  
};

typedef std::queue<Command> CommandQueue;