#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"
#include "ResourceManager.h"
#include "LevelManager.h"
#include "Game.h"
#include "State.h"
#include "Constants.h"
#include "SaveManager.h"
#include "MusicPlayer.h"


Game::Game() : 
        mWindow(sf::RenderWindow(sf::VideoMode(1024,768), "Marvin", sf::Style::Close)),
        mPlayer(),
        mTextureManager(),
        mFontManager(),
        mMusicPlayer(),
        mSoundPlayer(),
        mLevelManager(mWindow, mSoundPlayer),
        mStateStack(State::Context(mWindow, mTextureManager, mFontManager, mLevelManager, mMusicPlayer, mSoundPlayer, mPlayer)){

    mWindow.setVerticalSyncEnabled(true);

    //Load necessary textures and fonts
    mTextureManager.load(TextureID::Background, "Resources/Textures/Background/bg.png");
    mTextureManager.load(TextureID::MenuGround, "Resources/Textures/Tileset/ground_repeat_texture.png");
    mTextureManager.load(TextureID::CastleBackground, "Resources/Textures/Background/bg_castle.png");
    mTextureManager.load(TextureID::GrasslandsBackground, "Resources/Textures/Background/bg_grasslands.png");
    mTextureManager.load(TextureID::GreenButton, "Resources/Textures/GUI/green_button.png");
    mTextureManager.load(TextureID::ButtonDisabled, "Resources/Textures/GUI/button_disabled.png");
    mTextureManager.load(TextureID::GreenButtonPressed, "Resources/Textures/GUI/green_button_pressed.png");
    mTextureManager.load(TextureID::PauseScreenPanel, "Resources/Textures/GUI/grey_panel.png");
    mTextureManager.load(TextureID::LevelCompletionPanel, "Resources/Textures/GUI/grey_panel_medium.png");
    mTextureManager.load(TextureID::LevelSelectionPanel, "Resources/Textures/GUI/grey_panel_large.png");
    mTextureManager.load(TextureID::GreenContainerButton, "Resources/Textures/GUI/green_panel_button.png");
    mTextureManager.load(TextureID::GreenContainerButtonPressed, "Resources/Textures/GUI/green_panel_button_pressed.png");
    mTextureManager.load(TextureID::ContainerButtonDisabled, "Resources/Textures/GUI/panel_button_disabled.png");
    mTextureManager.load(TextureID::PlayerSpriteSheet, "Resources/Textures/Player/player_spritesheet.png");
    mTextureManager.load(TextureID::EnemiesSpriteSheet, "Resources/Textures/Enemy/enemies_spritesheet.png");
    mFontManager.load(FontID::Main, "Resources/Fonts/kenvector_future.ttf");
    mFontManager.load(FontID::Thin, "Resources/Fonts/kenvector_future_thin.ttf");
    mStateStack.pushState(State::ID::Menu);

    //Load game data    
    load("save.bin", SaveManager::getInstance());
}

void Game::run(){

    sf::Time elapsedTime = sf::Time::Zero;
    sf::Clock clock;
    while (mWindow.isOpen()){
        elapsedTime += clock.restart();
        while (elapsedTime > FRAME_RATE){

            elapsedTime -= FRAME_RATE;
            handleInput();
            update(FRAME_RATE);

            if (mStateStack.isEmpty()){
                exit();
            }
        }
        draw();
    }

}

void Game::handleInput(){

    sf::Event event;
    while (mWindow.pollEvent(event)){
        mStateStack.handleEvent(event);
        if (event.type == sf::Event::Closed){
            exit();
        }
    }
}

void Game::update(sf::Time deltaTime){
    mStateStack.update(deltaTime);
}

void Game::draw(){
    mWindow.clear();
    mStateStack.draw();
    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}

void Game::exit(){

    //Save data
    save("save.bin", SaveManager::getInstance());
    mWindow.close();
}

void Game::load(const std::string &saveFile, SaveManager &saveManager){
    std::ifstream ifs(saveFile.c_str(), std::ios::binary);
    if (ifs.good()){
        boost::archive::binary_iarchive ia(ifs);
        ia >> saveManager;
        int test = 5;
    }

    //Create save file if nonexistant
    else{
        std::ofstream ofs(saveFile.c_str(), std::ios::binary);
        boost::archive::binary_oarchive oa(ofs);
        saveManager.makeNewSave();
        oa << saveManager;
    }
}

void Game::save(const std::string &saveFile, SaveManager &saveManager){
    std::ofstream ofs(saveFile.c_str(), std::ios::binary);
    boost::archive::binary_oarchive oa(ofs);
    oa << saveManager;
}