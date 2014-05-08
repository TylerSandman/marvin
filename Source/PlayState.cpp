#include "PlayState.h"

PlayState::PlayState(StateStack &stack, Context context, const std::string &map) : 
        State(stack, context),
        mPlayer(*context.player){

    mWorld.reset(new World(*context.window, map));
}

void PlayState::draw(){
    mWorld->draw();
}

bool PlayState::update(sf::Time deltaTime){

    mWorld->update(deltaTime);
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleRealtimeInput(commands);
    return true;
}

bool PlayState::handleEvent(const sf::Event &event){
    
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleEvent(event, commands);

    if ((event.type == sf::Event::KeyPressed) && 
        (event.key.code == sf::Keyboard::P)){
        requestStackPush(ID::Pause);
    }
    return true;
}