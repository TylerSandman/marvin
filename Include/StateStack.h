#pragma once
#include "State.h"

class StateStack{

public:
    enum Action{
        Push,
        Pop,
        Clear,
        Reload
    };

public:
    StateStack(State::Context context);
    void update(sf::Time deltaTime);
    void draw();
    void handleEvent(const sf::Event &event);
    State::Ptr createState(State::ID stateID, const std::string &map="grasslands.json");
    void pushState(State::ID stateID, const std::string &map="grasslands.json");
    void popState();
    void clearStates();
    void reloadStates();
    bool isEmpty() const;

private:
    void applyPendingChanges();

private:
    struct PendingChange{
        PendingChange(StateStack::Action action, State::ID stateID=State::ID::None, const std::string &map="grasslands.json") : 
            action(action), stateID(stateID), map(map){}
        StateStack::Action action;
        State::ID stateID;
        std::string map;
    };


private:
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;
    State::Context mContext;
};