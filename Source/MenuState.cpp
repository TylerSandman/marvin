#include <memory>
#include "MenuState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

MenuState::MenuState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(){

    sf::Vector2f windowSize(context.window->getSize());

    //Buttons
    auto playButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    playButton->setCallback(
        [this]() {
            requestStackPop();
            requestStackPush(ID::LevelSelect);
        });
    playButton->setText("Play", sf::Color::Black);
    playButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f - 70));
    mGUIContainer.add(playButton);
    
    auto optionsButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    optionsButton->setCallback(
        [this]() {
            requestStackClear();
            requestStackPush(ID::LevelSelect);
        });
    optionsButton->setText("Options", sf::Color::Black);
    optionsButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f));
    mGUIContainer.add(optionsButton);

    auto exitButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    exitButton->setCallback(
        [this]() {
            exit(0);
        });
    exitButton->setText("Exit", sf::Color::Black);
    exitButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f + 70));
    mGUIContainer.add(exitButton);
}

void MenuState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time deltaTime){
    return false;
}

bool MenuState::handleEvent(const sf::Event &event){
    mGUIContainer.handleEvent(event);
    return false;
}