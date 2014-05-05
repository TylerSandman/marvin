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
#include "World.h"
#include "Constants.h"

Game::Game(const std::string &map) : 
        mWindow(sf::RenderWindow(sf::VideoMode(1024,768), "Marvin", sf::Style::Close)),
        mView(mWindow.getDefaultView()){
        
    mWorld.reset(new World(mWindow, map));
    mWindow.setVerticalSyncEnabled(true);
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
        }
        draw();
    }
}

void Game::handleInput(){

    CommandQueue &commands = mWorld->getCommandQueue();

    sf::Event event;
    while (mWindow.pollEvent(event)){

        mPlayer.handleEvent(event, commands);

        if (event.type == sf::Event::Closed){
            mWindow.close();
        }
    }

    mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time deltaTime){
    mWorld->update(deltaTime);
}

void Game::draw(){

    mWindow.clear();
    mWorld->draw();
    mWindow.display();
}