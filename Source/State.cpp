#include "State.h"
#include "StateStack.h"

State::State(StateStack &stack, Context context) :
    mStack(&stack), mContext(context){}

State::~State(){}

void State::requestStackPush(ID stateID, const std::string &map){
    mStack->pushState(stateID, map);
}

void State::requestStackPop(){
    mStack->popState();
}

void State::requestStackClear(){
    mStack->clearStates();
}

void State::requestStackReload(){
    mStack->reloadStates();
}

State::Context State::getContext() const{ return mContext; }