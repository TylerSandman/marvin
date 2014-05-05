#pragma once
#include "State.h"

class StateStack{

public:
    enum Action{
        Push,
        Pop,
        Clear
    };

public:
    StateStack(State::Context context);
    void update(sf::Time deltaTime);
    void draw();
    void handleEvent(const sf::Event &event);
    State::Ptr createState(State::ID stateID);
    void pushState(State::ID stateID);
    void popState();
    void clearStates();
    bool isEmpty() const;

private:
    void applyPendingChanges();

private:
    struct PendingChange{
        PendingChange(StateStack::Action action, State::ID stateID=State::ID::None) : 
            action(action), stateID(stateID){}
        StateStack::Action action;
        State::ID stateID;
    };


private:
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;
    State::Context mContext;
};