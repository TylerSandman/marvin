#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"

class StateStack;

class HelpState : public State{

public:
    HelpState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);
    void onResolutionChange();

private:
    GUI::Container mGUIContainer;
};