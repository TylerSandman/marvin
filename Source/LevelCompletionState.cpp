#include <memory>
#include <boost/math/special_functions/round.hpp>
#include <iostream>
#include "LevelCompletionState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "Number.h"
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

    GUI::Image::Ptr clock = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::HUDSpriteSheet),
        sf::IntRect(48, 140, 48, 48));

    GUI::Number::Ptr time = std::make_shared<GUI::Number>(
        *getContext().textureManager, static_cast<float>(data.time));

    GUI::Image::Ptr badge = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::HUDSpriteSheet),
        sf::IntRect(54, 49, 48, 48));

    float mapBadgeTime = SaveManager::getInstance().getLevelData(data.map).badgeTime;
    GUI::Number::Ptr badgeTime = std::make_shared<GUI::Number>(
        *getContext().textureManager, static_cast<float>(mapBadgeTime));

    GUI::Image::Ptr heart = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::HUDSpriteSheet),
        sf::IntRect(0, 94, 53, 45));

    GUI::Number::Ptr deaths = std::make_shared<GUI::Number>(
        *getContext().textureManager, static_cast<unsigned int>(data.deaths));

    mGUIContainer.add(titleLabel);
    mGUIContainer.add(clock);
    mGUIContainer.add(time);
    mGUIContainer.add(badge);
    mGUIContainer.add(badgeTime);
    mGUIContainer.add(deaths);
    mGUIContainer.add(heart);

    titleLabel->setPosition(
        windowSize.x / 2.f,
        windowSize.y / 2.f - 200.f);

    clock->setPosition(
        windowSize.x / 2.f - 100.f,
        windowSize.y / 2.f);

    time->setPosition(
        windowSize.x / 2.f + 100.f,
        windowSize.y / 2.f);

    badge->setPosition(
        windowSize.x / 2.f - 100.f,
        windowSize.y / 2.f + 100.f);

    badgeTime->setPosition(
        windowSize.x / 2.f + 100.f,
        windowSize.y / 2.f + 100.f);
    
    heart->setPosition(
        windowSize.x / 2.f - 100.f,
        windowSize.y / 2.f + 200.f);

    deaths->setPosition(
        windowSize.x / 2.f + 120.f,
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
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space){
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