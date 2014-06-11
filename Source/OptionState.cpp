#include <memory>
#include "OptionState.h"
#include "Container.h"
#include "OptionContainer.h"
#include "Image.h"
#include "Button.h"
#include "ResourceManager.h"

OptionState::OptionState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()){

    //Background panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::OptionScreenPanel));
    backgroundPanel->setPosition(windowSize * 0.5f);
    mGUIContainer.add(backgroundPanel);

    //Options container
    auto optionsContainer = std::make_shared<GUI::OptionContainer>(
        context);
    mGUIContainer.add(optionsContainer);
    optionsContainer->activate();
    optionsContainer->setPosition(windowSize * 0.5f);

    auto soundOption = std::make_shared<GUI::Option>(
        "Music", *getContext().fontManager);
    soundOption->addPair(std::make_pair("0", 0));
    soundOption->addPair(std::make_pair("100", 100));
    soundOption->setCallback(
        [this](int volume){
            getContext().musicPlayer->setVolume(volume);
    });
    optionsContainer->add(soundOption);

    mOptionContainer = optionsContainer;
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
        else if (event.key.code == sf::Keyboard::Return)
            applyOptions();
    }
    mGUIContainer.handleEvent(event);
    return false;
}

void OptionState::applyOptions(){
    mOptionContainer->apply();
}
