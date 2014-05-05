#pragma once
#include "State.h"
#include "World.h"

class StateStack;

class PlayState : public State{

public:
    PlayState(StateStack &stack, Context context, const std::string &map);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);

private:
    std::unique_ptr<World> mWorld;
    Player &mPlayer;
};