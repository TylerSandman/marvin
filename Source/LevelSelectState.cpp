#include <memory>
#include <boost/math/special_functions/round.hpp>
#include <iostream>
#include "LevelSelectState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "ContainerButton.h"
#include "SaveManager.h"

LevelSelectState::LevelSelectState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context.window->getDefaultView()),
        numLevels(0){

    sf::Vector2u windowSize = context.window->getSize();

    //Background
    sf::Texture &bgTexture = getContext().textureManager->get(TextureID::Background);
    sf::IntRect bgRect(
        0,
        0,
        static_cast<int>(windowSize.x),
        static_cast<int>(windowSize.y));
    bgTexture.setRepeated(true);
    mBackground = sf::Sprite(bgTexture, bgRect);

    //Backdrop
    sf::Texture &backdropTexture = getContext().textureManager->get(TextureID::GrasslandsBackground);
    sf::IntRect backdropRect(
        0, 0,
        static_cast<int>(windowSize.x),
        backdropTexture.getSize().y);
    backdropTexture.setRepeated(true);
    mBackdrop = sf::Sprite(backdropTexture, backdropRect);
    mBackdrop.setPosition(
        0, 
        windowSize.y - static_cast<float>(backdropTexture.getSize().y));

    //Panel to display levels
    buildLevelPanel();

    //Play main theme if we didn't get here from the menu state
    if (!context.musicPlayer->isPlaying())
        context.musicPlayer->play(MusicID::MainTheme);
}

void LevelSelectState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mBackground);
    window.draw(mBackdrop);
    window.draw(mLevelPanel);
    window.draw(mGUIContainer);
}

bool LevelSelectState::update(sf::Time deltaTime){
    return false;
}

bool LevelSelectState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        mGUIContainer.handleEvent(event);
        if (event.key.code == sf::Keyboard::Escape){
            requestStackPop();
            requestStackPush(ID::Menu);
        }
    }
    return false;
}

void LevelSelectState::addLevel(LevelData lData, const std::string &map, bool enabled){

    sf::Vector2f windowSize(getContext().window->getSize());
    auto levelButton = std::make_shared<GUI::ContainerButton>(
         *getContext().textureManager);

    levelButton->setCallback(
        [this, map]() {
            requestStackPop();
            requestStackPush(State::ID::Loading, map);
        });

    GUI::Label::Ptr levelLabel;
    if (enabled){
        levelLabel = std::make_shared<GUI::Label>(
            lData.name,
            sf::Color::Black,
            32,
            *getContext().fontManager);

        GUI::Label::Ptr timeLabel;
        std::ostringstream timeStream;
        timeStream << lData.bestTime;
        timeLabel = std::make_shared<GUI::Label>(
            timeStream.str(),
            sf::Color::Black,
            32,
            *getContext().fontManager);
    }
    else{
        levelLabel = std::make_shared<GUI::Label>(
            lData.name,
            sf::Color(0,0,0,100),
            32,
            *getContext().fontManager);
        levelButton->disable();
    }  

    GUI::Label::Ptr timeLabel;

    if (lData.bestTime > 0.0f){
        std::ostringstream timeStream;
        timeStream << lData.bestTime;
        timeLabel = std::make_shared<GUI::Label>(
            timeStream.str(),
            sf::Color::Black,
            32,
            *getContext().fontManager);
    }
    else{
        std::ostringstream timeStream;
        timeStream << lData.bestTime;
        timeLabel = std::make_shared<GUI::Label>(
            timeStream.str(),
            sf::Color(0,0,0,100),
            32,
            *getContext().fontManager);
    }
    
    levelButton->add(levelLabel);
    levelButton->add(timeLabel);
    mGUIContainer.add(levelButton);
    float posX = windowSize.x * 0.5f;
    float posY = static_cast<float>(125 + 100 * numLevels);
    levelButton->setPosition(posX, posY);

    //Kind of a hacky way to center the text, but this
    //is to compensate for the padding the selection arrow gives
    levelLabel->move(20, -10);
    timeLabel->move(20, -10);
    timeLabel->move(300, 0);

    ++numLevels;
}

void LevelSelectState::buildLevelPanel(){

    sf::Vector2f windowSize(getContext().window->getSize());
    sf::Texture &bgPanelTexture = getContext().textureManager->get(TextureID::LevelSelectionPanel);
    mLevelPanel = sf::Sprite(bgPanelTexture);
    mLevelPanel.setOrigin(sf::Vector2f(
        mLevelPanel.getGlobalBounds().width / 2,
        mLevelPanel.getGlobalBounds().height / 2));
    mLevelPanel.setPosition(windowSize * 0.5f);
    mLevelPanel.setColor(sf::Color(255, 255, 255, 100));

    //Set a centered view for our level display panel to handle
    //overflowing levels and scrolling down
    sf::View levelPanelView;
    levelPanelView.setCenter(mLevelPanel.getPosition());
    levelPanelView.setSize(sf::Vector2f(
        static_cast<float>(bgPanelTexture.getSize().x),
        static_cast<float>(bgPanelTexture.getSize().y)));

    //Set viewport ratio according to the ratio of panel size
    //to window size, with a small amount of padding
    levelPanelView.setViewport(sf::FloatRect(
        (windowSize.x - bgPanelTexture.getSize().x) / 2.f / windowSize.x,
        ((windowSize.y - bgPanelTexture.getSize().y) / 2.f / windowSize.y) + 0.01f,
        bgPanelTexture.getSize().x / windowSize.x,
        bgPanelTexture.getSize().y / windowSize.y - 0.02f));

    mGUIContainer = GUI::Container(levelPanelView);


    SaveManager &saveManager = SaveManager::getInstance();

    //Level selection buttons
    LevelData data = saveManager.getLevelData("grasslands.json");
    addLevel(data, "grasslands.json", true);

    LevelData previousData = data;
    data = saveManager.getLevelData("testmap.json");   
    addLevel(data, "testmap.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("waterboy.json");
    addLevel(data, "waterboy.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("clearwalk.json");
    addLevel(data, "clearwalk.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("gofast.json");
    addLevel(data, "gofast.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("slowdown.json");
    addLevel(data, "slowdown.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("highheights.json");
    addLevel(data, "highheights.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("hotpursuit.json");
    addLevel(data, "hotpursuit.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("dangahzone.json");
    addLevel(data, "dangahzone.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("dangahzone2.json");
    addLevel(data, "dangahzone2.json", previousData.completed);

    previousData = data;
    data = saveManager.getLevelData("dangahzone3.json");
    addLevel(data, "dangahzone3.json", previousData.completed);
}