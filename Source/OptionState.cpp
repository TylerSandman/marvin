#include <memory>
#include "OptionState.h"
#include "Container.h"
#include "OptionContainer.h"
#include "Image.h"
#include "Button.h"
#include "ResourceManager.h"
#include "BoolOption.h"
#include "IntOption.h"
#include "VectorOption.h"

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

    
    auto soundOption = std::make_shared<GUI::IntOption>(
        "Music", *getContext().fontManager);
    for (int i = 0; i <= 20; ++i){
        int volumeVal = 5*i;
        std::ostringstream convert;
        convert << volumeVal;
        soundOption->addPair(std::make_pair(convert.str(), volumeVal));
    }
    soundOption->setCallback(
        [this](int volume){
            getContext().musicPlayer->setVolume(volume);
    });
    soundOption->select(
        static_cast<int>(getContext().musicPlayer->getVolume() / 5.f));
    optionsContainer->add(soundOption);

    auto fxOption = std::make_shared<GUI::IntOption>(
        "Effects", *getContext().fontManager);
    for (int i = 0; i <= 20; ++i){
        int volumeVal = 5*i;
        std::ostringstream convert;
        convert << volumeVal;
        fxOption->addPair(std::make_pair(convert.str(), volumeVal));
    }
    fxOption->setCallback(
        [this](int volume){
            getContext().soundPlayer->setVolume(volume);
    });
    fxOption->select(
        static_cast<int>(getContext().soundPlayer->getVolume() / 5.f));
    optionsContainer->add(fxOption);

    auto resolutionOption = std::make_shared<GUI::VectorOption>(
        "Resolution", *getContext().fontManager);
    resolutionOption->addPair(std::make_pair("1024 x 768", sf::Vector2f(1024, 768)));
    resolutionOption->addPair(std::make_pair("1920 x 1080", sf::Vector2f(1920, 1080)));
    resolutionOption->setCallback(
        [this](sf::Vector2f resolution){
            getContext().window->create(sf::VideoMode(
                static_cast<int>(resolution.x),
                static_cast<int>(resolution.y)), "Marvin", sf::Style::Close);
    });
    optionsContainer->add(resolutionOption);
    

    auto fullscreenOption = std::make_shared<GUI::BoolOption>(
        "Fullscreen", *getContext().fontManager);
    fullscreenOption->addPair(std::make_pair("On", true));
    fullscreenOption->addPair(std::make_pair("Off", false));
    fullscreenOption->setCallback(
        [this](bool fullscreen){
            sf::Vector2u windowSize = getContext().window->getSize();
            sf::VideoMode mode(windowSize.x, windowSize.y);
            if (!mode.isValid()) return;
            fullscreen ? getContext().window->create(sf::VideoMode(windowSize.x, windowSize.y), "Marvin", sf::Style::Fullscreen) : 
                         getContext().window->create(sf::VideoMode(windowSize.x, windowSize.y), "Marvin", sf::Style::Close);
        requestStackPop();
        requestStackPush(ID::Option);
    });
    fullscreenOption->select(1);
    optionsContainer->add(fullscreenOption);

    soundOption->move(0.f, -85.f);
    fxOption->move(0.f, -35.f);
    resolutionOption->move(0.f, 15.f);
    fullscreenOption->move(0.f, 65.f);
    
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
    triggerResolutionChange();
}

void OptionState::onResolutionChange(){}