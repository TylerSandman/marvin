#pragma once
#include "State.h"
#include "World.h"
#include "Container.h"
#include "AnimatedSprite.h"

class StateStack;

class MenuState : public State{

public:
    MenuState(StateStack &stack, Context context);
    void draw();
    bool update(sf::Time deltaTime);
    bool handleEvent(const sf::Event &event);
    ID getID() const;

private:
    GUI::Container mGUIContainer;
    sf::Text mTitleText;
    sf::Sprite mGrass;
    sf::Sprite mBackground;
    sf::Sprite mBackdrop;
    Animation mCharacterAnimation;
    AnimatedSprite mCharacter;
    ID mID;
};