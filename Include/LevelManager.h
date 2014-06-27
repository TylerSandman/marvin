#pragma once
#include <SFML\Graphics.hpp>
#include <map>
#include <string>
#include "SoundPlayer.h"
#include "State.h"

class World;

class LevelManager {

public:
    LevelManager();
    void insert(State::Context context, std::string &map);
    void load(const std::string &map);
    World* get(const std::string &map);

private:
    std::map<std::string, World*> mLevelMap;
};