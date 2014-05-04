#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "World.h"

class Game{

public:
    Game(const std::string &map);
    void run();

private:
    void handleInput();
    void update(sf::Time deltaTime);
    void draw();

private:
    sf::RenderWindow mWindow;
    sf::View mView;
    std::unique_ptr<World> mWorld;
};