#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"

class StateStack;

class LevelSelectState : public State{

public:
    LevelSelectState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);

private:
    void addLevel(const std::string &name, bool enabled=true);

private:
    GUI::Container mGUIContainer;
    sf::Sprite mBackground;
    unsigned int numLevels;
};