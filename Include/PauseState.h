#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"

class StateStack;

class PauseState : public State{

public:
    PauseState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);

private:
    GUI::Container mGUIContainer;
};