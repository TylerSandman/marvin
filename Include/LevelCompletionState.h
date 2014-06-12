#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"
#include "SaveManager.h"

class StateStack;

class LevelCompletionState : public State{

public:
    LevelCompletionState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);
    void onResolutionChange(); 

private:
    GUI::Container mGUIContainer;
};