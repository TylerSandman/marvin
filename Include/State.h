#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include "Player.h"

class StateStack;

class State{

public:
    typedef std::unique_ptr<State> Ptr;
    struct Context{
        Context(sf::RenderWindow &window, TextureManager &textureManager, Player &player, const std::string &map="") : 
            window(&window), textureManager(&textureManager), player(&player), map(map){}
        sf::RenderWindow *window;
        TextureManager *textureManager;
        Player *player;
        std::string map;
    };
    enum ID{
        None,
        Title,
        Play,
        Pause
    };

public:
    State(StateStack &stack, Context context);
    virtual ~State();
    virtual void draw() = 0;
    virtual bool update(sf::Time deltaTime) = 0;
    virtual bool handleEvent(const sf::Event &event) = 0;

protected:
    void requestStackPush(ID stateID);
    void requestStackPop();
    void requestStackClear();
    Context getContext() const;

private:
    StateStack *mStack;
    Context mContext;
};