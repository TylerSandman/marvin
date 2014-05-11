#include <memory>
#include "LevelSelectState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "ContainerButton.h"

LevelSelectState::LevelSelectState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(),
        numLevels(0){

    //Background
    sf::Texture &bgTexture = getContext().textureManager->get(TextureID::Background);
    sf::IntRect textureRect(
        0,
        0,
        static_cast<int>(getContext().window->getSize().x),
        static_cast<int>(getContext().window->getSize().y));
    bgTexture.setRepeated(true);
    mBackground = sf::Sprite(bgTexture, textureRect);

    //Background panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::LevelSelectionPanel));
    backgroundPanel->setPosition(windowSize * 0.5f);
    mGUIContainer.add(backgroundPanel);

    //Level selection buttons
    addLevel("Grasslands");
    addLevel("Jumping Fun", false);
    addLevel("Waterboy", false);
    addLevel("Clear Walk", false);
    addLevel("Go Fast", false);
    addLevel("Slow Down", false);
    addLevel("High Heights", false);
    addLevel("Hot Pursuit", false);
    addLevel("Dangah Zone", false);
}

void LevelSelectState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mBackground);
    window.draw(mGUIContainer);
}

bool LevelSelectState::update(sf::Time deltaTime){
    return false;
}

bool LevelSelectState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        ;//TODO handle menu navigation
    }
    mGUIContainer.handleEvent(event);
    return false;
}

void LevelSelectState::addLevel(const std::string &name, bool enabled){
    sf::Vector2f windowSize(getContext().window->getSize());
    auto levelButton = std::make_shared<GUI::ContainerButton>(
         *getContext().textureManager);
    levelButton->setCallback(
        [this]() {
            requestStackPop();
            requestStackPush(State::ID::Play);
        });
    GUI::Label::Ptr levelLabel;
    if (enabled){
        levelLabel = std::make_shared<GUI::Label>(
            name,
            sf::Color::Black,
            *getContext().fontManager);
    }
    else{
        levelLabel = std::make_shared<GUI::Label>(
            name,
            sf::Color(0,0,0,100),
            *getContext().fontManager);
        levelButton->disable();
    }
    float posX = windowSize.x*0.5f + 250*((numLevels % 3) -1.f);
    float posY = windowSize.y*0.5f + 200*((numLevels / 3) -1.f);
    levelButton->setPosition(posX, posY);
    levelButton->add(levelLabel);
    mGUIContainer.add(levelButton);
    ++numLevels;
}