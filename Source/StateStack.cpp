#include "StateStack.h"
#include "PlayState.h"

StateStack::StateStack(State::Context context): mContext(context){}

void StateStack::update(sf::Time deltaTime){

    for(auto i = mStack.rbegin(); i != mStack.rend(); ++i){
        if (!i->get()->update(deltaTime))
            return;
    }
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
    applyPendingChanges();
}

State::Ptr StateStack::createState(State::ID stateID){
    switch(stateID){
    case(State::ID::Play):
        return std::unique_ptr<PlayState>(new PlayState(*this, mContext, mContext.map));
    }
}

void StateStack::pushState(State::ID stateID){
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState(){
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates(){
    mPendingList.push_back(PendingChange(Clear));
}
    
bool StateStack::isEmpty() const{
    return mStack.empty();
}

void StateStack::applyPendingChanges(){

    for(auto &change : mPendingList){

        switch(change.action){
        case(Action::Push):
            mStack.push_back(std::move(createState(change.stateID)));
            break;
        case(Action::Pop):
            mStack.pop_back();
            break;
        case(Action::Clear):
            mStack.clear();
            break;
        }
    }
    mPendingList.clear();
}