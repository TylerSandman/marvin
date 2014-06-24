#include <memory>
#include "HelpState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

HelpState::HelpState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()){

    //Background panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::HelpScreenPanel));
    mGUIContainer.add(backgroundPanel);
    mGUIContainer.setPosition(windowSize * 0.5f);
}

void HelpState::draw(){
    sf::RenderWindow &window = *getContext().window;
    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, window.getSize().y));
    bg.setFillColor(sf::Color(0,0,0,155));
    bg.setPosition(0,0);
    window.draw(bg);
    window.setView(window.getDefaultView());
    mGUIContainer.setView(getContext().window->getDefaultView());
    window.draw(mGUIContainer);
}

bool HelpState::update(sf::Time deltaTime){
    return true;
}

bool HelpState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        if ((event.key.code == sf::Keyboard::P) ||
            (event.key.code == sf::Keyboard::Escape) || 
            (event.key.code == sf::Keyboard::Space) ||
            (event.key.code == sf::Keyboard::Return))
            requestStackPop();
    }
    return false;
}

void HelpState::onResolutionChange(){}