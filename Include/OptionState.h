#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"
#include "OptionContainer.h"

class StateStack;

class OptionState : public State{

public:
    OptionState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);
    void applyOptions();
    void onResolutionChange();

private:
    GUI::Container mGUIContainer;
    GUI::OptionContainer::Ptr mOptionContainer;   
};