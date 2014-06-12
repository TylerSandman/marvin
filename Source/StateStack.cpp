#include "StateStack.h"
#include "PlayState.h"
#include "PauseState.h"
#include "LevelSelectState.h"
#include "LevelLoadingState.h"
#include "LevelCompletionState.h"
#include "MenuState.h"
#include "OptionState.h"

StateStack::StateStack(State::Context context): mContext(context){}

void StateStack::update(sf::Time deltaTime){

    for(auto i = mStack.rbegin(); i != mStack.rend(); ++i){
        if (!i->get()->update(deltaTime))
            break;
    }
    applyPendingChanges();
}

void StateStack::draw(){

    for(auto i = mStack.begin(); i != mStack.end(); ++i){
        i->get()->draw();
    }
}

void StateStack::handleEvent(const sf::Event &event){

    for(auto i = mStack.rbegin(); i != mStack.rend(); ++i){
        if (!i->get()->handleEvent(event))
            break;
    }
}

State::Ptr StateStack::createState(State::ID stateID, const std::string &map){

    switch(stateID){
    case(State::ID::Play):
        return std::unique_ptr<PlayState>(new PlayState(*this, mContext, map));
    case(State::ID::Pause):
        return std::unique_ptr<PauseState>(new PauseState(*this, mContext));
    case(State::ID::LevelSelect):
        return std::unique_ptr<LevelSelectState>(new LevelSelectState(*this, mContext));
    case(State::ID::LevelCompletion):
        return std::unique_ptr<LevelCompletionState>(new LevelCompletionState(*this, mContext));
    case(State::ID::Loading):
        return std::unique_ptr<LevelLoadingState>(new LevelLoadingState(*this, mContext, map));
    case(State::ID::Menu):
        return std::unique_ptr<MenuState>(new MenuState(*this, mContext));
    case(State::ID::Option):
        return std::unique_ptr<OptionState>(new OptionState(*this, mContext));
    default:
        return std::unique_ptr<MenuState>(new MenuState(*this, mContext));
    }
}

void StateStack::pushState(State::ID stateID, const std::string &map){
    mPendingList.push_back(PendingChange(Push, stateID, map));
}

void StateStack::popState(){
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates(){
    mPendingList.push_back(PendingChange(Clear));
}

void StateStack::reloadStates(){
    mPendingList.push_back(PendingChange(Reload));
}
    
bool StateStack::isEmpty() const{
    return mStack.empty();
}

void StateStack::applyPendingChanges(){

    for(auto &change : mPendingList){

        switch(change.action){
        case(Action::Push):
            mStack.push_back(std::move(createState(change.stateID, change.map)));
            break;
        case(Action::Pop):
            mStack.pop_back();
            break;
        case(Action::Clear):
            mStack.clear();
            break;
        case(Action::Reload):
            std::vector<State::ID> reloadIDs;
            for (auto &state : mStack){
                reloadIDs.push_back(state->getID());
            }
            for (auto &id : reloadIDs){
                mStack.push_back(std::move(createState(id)));
            }
            break;
        }
    }
    mPendingList.clear();
}