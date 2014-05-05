#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "StateStack.h"
#include "Player.h"
#include "ResourceManager.h"

class Game{

public:
    Game();
    void run();

private:
    void handleInput();
    void update(sf::Time deltaTime);
    void draw();

private:
    sf::RenderWindow mWindow;
    Player mPlayer;
    TextureManager mTextureManager;
    StateStack mStateStack;
};