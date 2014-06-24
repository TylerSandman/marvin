#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include "LevelManager.h"
#include "Player.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

class StateStack;
class World;

class State{

public:
    typedef std::unique_ptr<State> Ptr;
    struct Context{
        Context(
            sf::RenderWindow &window, 
            TextureManager &textureManager, 
            FontManager &fontManager,
            LevelManager &levelManager,
            MusicPlayer &musicPlayer,
            SoundPlayer &soundPlayer,
            Player &player) : 
                window(&window), 
                textureManager(&textureManager), 
                fontManager(&fontManager), 
                levelManager(&levelManager),
                musicPlayer(&musicPlayer),
                soundPlayer(&soundPlayer),
                player(&player){}
        sf::RenderWindow *window;
        TextureManager *textureManager;
        FontManager *fontManager;
        LevelManager *levelManager;
        MusicPlayer *musicPlayer;
        SoundPlayer *soundPlayer;
        Player *player;
    };
    enum ID{
        None,
        Title,
        Play,
        Pause,
        LevelSelect,
        LevelCompletion,
        Loading,
        Option,
        Menu,
        Help
    };

public:
    State(StateStack &stack, Context context);
    virtual ~State();
    virtual void draw() = 0;
    virtual bool update(sf::Time deltaTime) = 0;
    virtual bool handleEvent(const sf::Event &event) = 0;
    virtual void onResolutionChange() = 0;

protected:
    void requestStackPush(ID stateID, const std::string &map="grasslands.json");
    void requestStackPop();
    void requestStackClear();
    void requestStackReload();
    void triggerResolutionChange();
    Context getContext() const;

private:
    StateStack *mStack;
    Context mContext;
};