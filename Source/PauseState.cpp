#include "PauseState.h"

PauseState::PauseState(StateStack &stack, Context context) : 
        State(stack, context){}

void PauseState::draw(){
}

bool PauseState::update(sf::Time deltaTime){
    return false;
}

bool PauseState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::P)
            requestStackPop();
    }
    return false;
}