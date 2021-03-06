#include "PlayState.h"
#include "LevelManager.h"
#include "SaveManager.h"

PlayState::PlayState(StateStack &stack, Context context, const std::string &map) : 
        State(stack, context),
        mPlayer(*context.player),
        mMapName(map){
    
    mWorld.reset(context.levelManager->get(mMapName));
    context.musicPlayer->play(MusicID::LevelTheme);
}

void PlayState::draw(){
    mWorld->draw();
}

bool PlayState::update(sf::Time deltaTime){

    if(!mWorld->isComplete())
        mWorld->update(deltaTime);
    else{
        requestStackPush(ID::LevelCompletion);
        SaveManager::getInstance().markLevelCompleted(mMapName, mWorld->getAttemptTime(), mWorld->getNumDeaths(), mWorld->isGemCollected());
    }
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleRealtimeInput(commands);  
  
    return false;
}

bool PlayState::handleEvent(const sf::Event &event){
    
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleEvent(event, commands);

    if ((event.type == sf::Event::KeyPressed) && 
        ((event.key.code == sf::Keyboard::P) || (event.key.code == sf::Keyboard::Escape))){
        requestStackPush(ID::Pause);
    }
    return false;
}

void PlayState::onResolutionChange(){
    mWorld->onResolutionChange();
}