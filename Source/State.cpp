#include "State.h"
#include "StateStack.h"

State::State(StateStack &stack, Context context) :
    mStack(&stack), mContext(context){}

State::~State(){}

void State::requestStackPush(ID stateID){
    mStack->pushState(stateID);
}

void State::requestStackPop(){
    mStack->popState();
}

void State::requestStackClear(){
    mStack->clearStates();
}

State::Context State::getContext() const{ return mContext; }