#pragma once
#include <SFML\Graphics.hpp>
#include <map>
#include <string>
#include "SoundPlayer.h"

class World;

class LevelManager {

public:
    LevelManager(sf::RenderWindow &window, SoundPlayer &soundPlayer);
    void insert(const std::string &map);
    void load(const std::string &map);
    World* get(const std::string &map);

private:
    std::map<std::string, World*> mLevelMap;
    sf::RenderWindow &mWindow;
    SoundPlayer &mSoundPlayer;
};