#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Command.h"
#include "Marvin.h"
#include "Constants.h"

class Player{

public:
    void handleEvent(const sf::Event &event, CommandQueue &commandQueue);
    void handleRealtimeInput(CommandQueue &commandQueue);
};