#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"
#include "SaveManager.h"

class StateStack;

class LevelSelectState : public State{

public:
    LevelSelectState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);
    void onResolutionChange();   

private:
    void addLevel(LevelData lData, const std::string &map, bool enabled=true);
    void buildLevelPanel();

private:
    GUI::Container mGUIContainer;
    sf::Sprite mLevelPanel;
    sf::Sprite mBackground;
    sf::Sprite mBackdrop;
    unsigned int numLevels;    
};