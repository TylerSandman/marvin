#include <memory>
#include "PauseState.h"
#include "Container.h"
#include "Panel.h"
#include "Button.h"

PauseState::PauseState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(){

    //Panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto buttonPanel = std::make_shared<GUI::Panel>(
        *getContext().textureManager,
        &mGUIContainer);
    buttonPanel->setPosition(windowSize * 0.5f);
    mGUIContainer.add(buttonPanel);

    //Buttons
    auto resumeButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager,
         buttonPanel.get());
    resumeButton->setOnClick(
        [this]() {
            requestStackPop();
        });
    resumeButton->setText("Resume", sf::Color::White);
    resumeButton->setPosition(0.f, -30.f);
    buttonPanel->add(resumeButton);
    
    auto exitButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager,
         buttonPanel.get());
    exitButton->setOnClick(
        [this]() {
            exit(0);
        });
    exitButton->setText("Exit", sf::Color::White);
    exitButton->setPosition(0.f, 30.f);
    buttonPanel->add(exitButton);
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