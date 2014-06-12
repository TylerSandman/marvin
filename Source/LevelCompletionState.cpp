#include <memory>
#include <boost/math/special_functions/round.hpp>
#include <iostream>
#include "LevelCompletionState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "ContainerButton.h"
#include "SaveManager.h"

LevelCompletionState::LevelCompletionState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()){

    sf::Vector2u windowSize = context.window->getSize();
    LevelCompletionData data = SaveManager::getInstance().getLastCompletedData();

    //Background panel where our stats will be displayed
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::LevelCompletionPanel));
    backgroundPanel->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    mGUIContainer.add(backgroundPanel);

    //Stats
    GUI::Label::Ptr titleLabel;
    titleLabel = std::make_shared<GUI::Label>(
        " Level Completed!",
        sf::Color::Black,
        36,
        *getContext().fontManager);

    GUI::Label::Ptr bestTimeLabel;
    float bestTime = SaveManager::getInstance().getLevelData(data.map).bestTime;
    std::ostringstream bestTimeStream;
    bestTimeStream << "Best: ";
    bestTimeStream << bestTime;
    if (data.time == bestTime)
        bestTimeStream << " new!";
    bestTimeLabel = std::make_shared<GUI::Label>(
        bestTimeStream.str(),
        (data.time == bestTime) ? sf::Color(111,196,169,255) : sf::Color::Black,
        32,
        *getContext().fontManager);

    GUI::Label::Ptr completionTimeLabel;
    std::ostringstream timeStream;
    timeStream << "Time: ";
    timeStream << data.time;
    completionTimeLabel = std::make_shared<GUI::Label>(
        timeStream.str(),
        sf::Color::Black,
        32,
        *getContext().fontManager);

    GUI::Label::Ptr deathsLabel;
    std::ostringstream deathStream;
    deathStream << "Deaths: ";
    deathStream << data.deaths;
    deathsLabel = std::make_shared<GUI::Label>(
        deathStream.str(),
        sf::Color::Black,
        32,
        *getContext().fontManager);

    mGUIContainer.add(titleLabel);
    mGUIContainer.add(completionTimeLabel);
    mGUIContainer.add(bestTimeLabel);
    mGUIContainer.add(deathsLabel);

    titleLabel->setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f - 200.f);

    completionTimeLabel->setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f);

    bestTimeLabel->setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f + 100.f);
    
    deathsLabel->setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f + 200.f);
}

void LevelCompletionState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}

bool LevelCompletionState::update(sf::Time deltaTime){
    return false;
}

bool LevelCompletionState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return){
            requestStackClear();
            requestStackPush(ID::LevelSelect);

            //Stop music so main theme will play
            getContext().musicPlayer->stop();
        }
    }
    mGUIContainer.handleEvent(event);
    return false;
}

void LevelCompletionState::onResolutionChange(){}