#include <memory>
#include "PauseState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

PauseState::PauseState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()){

    //Background panel where our button list will be displayed
    sf::Vector2f windowSize(context.window->getSize());
    auto backgroundPanel = std::make_shared<GUI::Image>(
        getContext().textureManager->get(TextureID::PauseScreenPanel));
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
    mGUIContainer.add(resumeButton);
    
    auto levelsButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    levelsButton->setCallback(
        [this]() {
            requestStackClear();
            requestStackPush(ID::LevelSelect);

            //Stop music to main theme will play
            getContext().musicPlayer->stop();
        });
    levelsButton->setText("Levels", sf::Color::Black);
    mGUIContainer.add(levelsButton);

    auto optionsButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    optionsButton->setCallback(
        [this]() {
            requestStackPush(ID::Option);
        });
    optionsButton->setText("Options", sf::Color::Black);
    mGUIContainer.add(optionsButton);

    mGUIContainer.setPosition(windowSize * 0.5f);
    resumeButton->move(0.f, -70.f);
    optionsButton->move(0.f, 70.f);

    //Music
    context.musicPlayer->setPaused(true);
}

void PauseState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    mGUIContainer.setView(getContext().window->getDefaultView());
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

void PauseState::onResolutionChange(){

    sf::Vector2f windowSize(getContext().window->getSize());
    mGUIContainer.setPosition(windowSize * 0.5f);
}