#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "StateStack.h"
#include "Player.h"
#include "ResourceManager.h"
#include "LevelManager.h"

class Game{

public:
    Game();
    void run();

private:
    void handleInput();
    void update(sf::Time deltaTime);
    void draw();
    void exit();

private:
    sf::RenderWindow mWindow;
    Player mPlayer;
    TextureManager mTextureManager;
    FontManager mFontManager;
    LevelManager mLevelManager;
    StateStack mStateStack;    
};