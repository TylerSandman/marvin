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
    std::vector<std::pair<std::string, sf::Vector2f>> resolutionOptionPairs;
    resolutionOptionPairs.push_back(std::make_pair("800 x 600", sf::Vector2f(800, 600)));
    resolutionOptionPairs.push_back(std::make_pair("1024 x 768", sf::Vector2f(1024, 768)));
    resolutionOptionPairs.push_back(std::make_pair("1152 x 648", sf::Vector2f(1152, 648)));
    resolutionOptionPairs.push_back(std::make_pair("1152 x 864", sf::Vector2f(1152, 864)));
    resolutionOptionPairs.push_back(std::make_pair("1280 x 720", sf::Vector2f(1280, 720)));
    resolutionOptionPairs.push_back(std::make_pair("1280 x 800", sf::Vector2f(1280, 800)));
    resolutionOptionPairs.push_back(std::make_pair("1280 x 960", sf::Vector2f(1280, 960)));
    resolutionOptionPairs.push_back(std::make_pair("1280 x 1024", sf::Vector2f(1280, 1024)));
    resolutionOptionPairs.push_back(std::make_pair("1360 x 768", sf::Vector2f(1360, 768)));
    resolutionOptionPairs.push_back(std::make_pair("1360 x 1024", sf::Vector2f(1360, 1024)));
    resolutionOptionPairs.push_back(std::make_pair("1366 x 768", sf::Vector2f(1366, 768)));
    resolutionOptionPairs.push_back(std::make_pair("1400 x 1050", sf::Vector2f(1400, 1050)));
    resolutionOptionPairs.push_back(std::make_pair("1440 x 900", sf::Vector2f(1440, 900)));
    resolutionOptionPairs.push_back(std::make_pair("1600 x 900", sf::Vector2f(1600, 900)));
    resolutionOptionPairs.push_back(std::make_pair("1600 x 1200", sf::Vector2f(1600, 1200)));
    resolutionOptionPairs.push_back(std::make_pair("1680 x 1050", sf::Vector2f(1680, 1050)));
    resolutionOptionPairs.push_back(std::make_pair("1776 x 1000", sf::Vector2f(1776, 1000)));
    resolutionOptionPairs.push_back(std::make_pair("1920 x 1080", sf::Vector2f(1920, 1080)));
    
    for (auto &pair : resolutionOptionPairs)
        resolutionOption->addPair(pair);

    sf::Vector2f resolution = sf::Vector2f(
        sf::VideoMode::getDesktopMode().width,
        sf::VideoMode::getDesktopMode().height);
    
    resolutionOption->select(0);
    for (int i = 0; i < resolutionOptionPairs.size(); ++i){
        if ((resolution.x == resolutionOptionPairs[i].second.x) && 
            (resolution.y == resolutionOptionPairs[i].second.y))
            resolutionOption->select(i);
    }

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