#include "PlayState.h"
#include "SaveManager.h"

PlayState::PlayState(StateStack &stack, Context context, const std::string &map) : 
        State(stack, context),
        mPlayer(*context.player),
        mMapName(map){
    
    mWorld.reset(context.levelManager->get(mMapName));
}

void PlayState::draw(){
    mWorld->draw();
}

bool PlayState::update(sf::Time deltaTime){

    mWorld->update(deltaTime);
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleRealtimeInput(commands);  

    if (mWorld->isComplete()){
        //TODO stats display etc
        requestStackPop();
        requestStackPush(ID::LevelSelect);
        SaveManager::getInstance().markLevelCompleted(mMapName);
    }
    return false;
}

bool PlayState::handleEvent(const sf::Event &event){
    
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleEvent(event, commands);

    if ((event.type == sf::Event::KeyPressed) && 
        (event.key.code == sf::Keyboard::P)){
        requestStackPush(ID::Pause);
    }
    return false;
}