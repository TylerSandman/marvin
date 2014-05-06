#include <memory>
#include "PauseState.h"
#include "Container.h"
#include "Button.h"

PauseState::PauseState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(){

    sf::Vector2f windowSize(context.window->getSize());
    auto resumeButton = std::make_shared<GUI::Button>(
        (*getContext().textureManager), &mGUIContainer);
    resumeButton->setCallback(
        [this]() {
            requestStackPop();
        });
    resumeButton->setText("Resume");
    resumeButton->setPosition(0.5f * windowSize.x, 0.5f * windowSize.y);
    mGUIContainer.add(resumeButton);
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