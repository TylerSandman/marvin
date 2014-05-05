#pragma once
#include <functional>
#include <queue>

class SceneNode;

namespace Category{
    enum Type{
        None = 0,
        Scene = 1 << 0,
        Player = 1 << 1
    };
}

struct Command{

    std::function<void(SceneNode&, sf::Time)> action;
    unsigned int category;  
};

typedef std::queue<Command> CommandQueue;