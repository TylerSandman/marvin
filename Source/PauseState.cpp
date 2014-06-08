#include <memory>
#include "PauseState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

PauseState::PauseState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context.window->getDefaultView()){

    //Background panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::PauseScreenPanel));
    backgroundPanel->setPosition(windowSize * 0.5f);
    mGUIContainer.add(backgroundPanel);

    //Buttons
    auto resumeButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    resumeButton->setCallback(
        [this]() {
            requestStackPop();
            getContext().musicPlayer->setPaused(false);
        });
    resumeButton->setText("Resume", sf::Color::Black);
    resumeButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f - 35));
    mGUIContainer.add(resumeButton);
    
    auto exitButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    exitButton->setCallback(
        [this]() {
            requestStackClear();
            requestStackPush(ID::LevelSelect);

            //Stop music to main theme will play
            getContext().musicPlayer->stop();
        });
    exitButton->setText("Levels", sf::Color::Black);
    exitButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f + 35));
    mGUIContainer.add(exitButton);

    //Music
    context.musicPlayer->setPaused(true);
}

void PauseState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time deltaTime){
    return false;
}

bool PauseState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::P)
            requestStackPop();
    }
    mGUIContainer.handleEvent(event);
    return false;
}