#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"
#include "ResourceManager.h"
#include "Game.h"
#include "State.h"
#include "Constants.h"

Game::Game() : 
        mWindow(sf::RenderWindow(sf::VideoMode(1024,768), "Marvin", sf::Style::Close)),
        mPlayer(),
        mTextureManager(),
        mStateStack(State::Context(mWindow, mTextureManager, mPlayer, "grasslands.json")){

    mWindow.setVerticalSyncEnabled(true);
    mStateStack.pushState(State::ID::Play);
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

            if (mStateStack.isEmpty())
                mWindow.close();
        }
        draw();
    }
}

void Game::handleInput(){

    sf::Event event;
    while (mWindow.pollEvent(event)){

        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed){
            mWindow.close();
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