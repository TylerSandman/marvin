#include <map>
#include <cassert>
#include "LevelManager.h"
#include "World.h"

LevelManager::LevelManager(){}

void LevelManager::insert(State::Context context, std::string &map){
    mLevelMap[map] = new World(context, map);
}

void LevelManager::load(const std::string &map){
    assert(mLevelMap[map] != nullptr);
    mLevelMap[map]->initialize();
}

World* LevelManager::get(const std::string &map){
    auto level = mLevelMap.find(map);
    assert(level != mLevelMap.end());
    return level->second;
}