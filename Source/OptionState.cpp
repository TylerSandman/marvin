#include <memory>
#include "OptionState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

OptionState::OptionState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()){

    //Background panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::OptionScreenPanel));
    backgroundPanel->setPosition(windowSize * 0.5f);
    mGUIContainer.add(backgroundPanel);
}

void OptionState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}

bool OptionState::update(sf::Time deltaTime){
    return true;
}

bool OptionState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Escape)
            requestStackPop();
    }
    mGUIContainer.handleEvent(event);
    return false;
}