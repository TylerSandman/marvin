#include "PlayState.h"
#include "SaveManager.h"

PlayState::PlayState(StateStack &stack, Context context, const std::string &map) : 
        State(stack, context),
        mPlayer(*context.player),
        mMapName(map),
        mID(ID::Play){
    
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
        //TODO stats display etc
        requestStackPush(ID::LevelCompletion);
        SaveManager::getInstance().markLevelCompleted(mMapName, mWorld->getAttemptTime(), mWorld->getNumDeaths());
    }
    CommandQueue &commands = mWorld->getCommandQueue();
    mPlayer.handleRealtimeInput(commands);  
  
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

State::ID PlayState::getID() const{
    return mID;
}