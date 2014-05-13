#pragma once
#include "State.h"
#include "World.h"

class StateStack;

class LevelLoadingState : public State{

public:
    LevelLoadingState(StateStack &stack, Context context, const std::string &map);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);

private:
    bool levelLoaded();
    void setLoadingText(const std::string &text);

private:
    World *mWorld;
    Player &mPlayer;
    sf::Thread mLoadingThread;
    sf::Mutex mMutex;
    sf::Text mLoadingText;
    std::string mMap;
    bool mMapLoaded;
    bool mObjectsLoaded;
    bool mTexturesLoaded;
};