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
    void addLevel(const std::string &name, const std::string &map, bool enabled=true);
    void buildLevelPanel();

private:
    sf::View mLevelPanelView;
    GUI::Container mGUIContainer;
    sf::Sprite mLevelPanel;
    sf::Sprite mBackground;
    sf::Sprite mBackdrop;
    unsigned int numLevels;
};