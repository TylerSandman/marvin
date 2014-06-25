#include <memory>
#include <boost/math/special_functions/round.hpp>
#include <iostream>
#include "LevelSelectState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "Number.h"
#include "ContainerButton.h"
#include "SaveManager.h"

LevelSelectState::LevelSelectState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()),
        mLevelContainer(std::make_shared<GUI::Container>(context, context.window->getDefaultView())),
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

    //Gem tracking
    sf::Texture &HUDTexture = context.textureManager->get(TextureID::HUDSpriteSheet);
    GUI::Image::Ptr cross = std::make_shared<GUI::Image>(
        HUDTexture, sf::IntRect(0, 239, 30, 28));
    GUI::Number::Ptr numGems = std::make_shared<GUI::Number>(
        *context.textureManager, getCollectedGems());
    GUI::Image::Ptr gem = std::make_shared<GUI::Image>(
        HUDTexture, sf::IntRect(98, 185, 46, 36));
    sf::Vector2f panelTopRight(
        windowSize.x * 0.5f + mLevelPanel.getGlobalBounds().width / 2.f,
        windowSize.y * 0.5f - mLevelPanel.getGlobalBounds().height / 2.f);
    mGUIContainer.add(cross);
    mGUIContainer.add(numGems);
    mGUIContainer.add(gem);
    cross->setPosition(panelTopRight);
    numGems->setPosition(panelTopRight);
    gem->setPosition(panelTopRight);
    cross->move(-135.f, 20);
    numGems->move(-90.f, 20);
    gem->move(-30.f, 20);

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
        mLevelContainer->handleEvent(event);
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
            28,
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
    levelLabel->setOrigin(0.f, levelLabel->getOrigin().y);

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

    GUI::Number::Ptr numGems = std::make_shared<GUI::Number>(
        *getContext().textureManager, lData.requiredGems);

    sf::Texture &HUDTexture = getContext().textureManager->get(TextureID::HUDSpriteSheet);
    sf::IntRect rect;

    if (lData.collectedGem)
        rect = sf::IntRect(98, 185, 46, 36);
    else
        rect = sf::IntRect(98, 97, 46, 36);
    GUI::Image::Ptr gem = std::make_shared<GUI::Image>(
        HUDTexture, rect);

    if (lData.badgeEarned)
        rect = sf::IntRect(54, 49, 48, 48);
    else
        rect = sf::IntRect(54, 0, 48, 48);
    GUI::Image::Ptr badge = std::make_shared<GUI::Image>(
        HUDTexture, rect);
    
    levelButton->add(levelLabel);
    levelButton->add(timeLabel);
    levelButton->add(numGems);
    levelButton->add(gem);
    levelButton->add(badge);
    mLevelContainer->add(levelButton);
    float posX = windowSize.x * 0.5f;
    float firstButtonPadding = windowSize.y / 2.f - mLevelPanel.getGlobalBounds().height / 2.f + 100.f;
    float posY = static_cast<float>(firstButtonPadding + 100 * numLevels);
    levelButton->setPosition(posX, posY);
    
    levelLabel->move(-270.f, -10);
    timeLabel->move(0, -10);
    timeLabel->move(280, 0);
    numGems->move(-330, 0);
    gem->move(130, 0);
    badge->move(180, 0);

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

    mLevelContainer->setView(levelPanelView);

    SaveManager &saveManager = SaveManager::getInstance();
    int collectedGems = getCollectedGems();

    //Level selection buttons
    std::vector<std::string> lPaths = saveManager.getLevelPaths();
    LevelData data = saveManager.getLevelData(lPaths[0]);
    addLevel(data, lPaths[0], true);
    LevelData previousData;
    for (auto it = lPaths.begin() + 1; it != lPaths.end(); ++it){
        previousData = data;
        data = saveManager.getLevelData(*it);
        addLevel(data, (*it), (previousData.completed) && (collectedGems >= data.requiredGems));
    }
    
    mGUIContainer.add(mLevelContainer);
}

int LevelSelectState::getCollectedGems(){

    SaveManager &saveManager = SaveManager::getInstance();
    int collectedGems = 0;
    for (auto &path : saveManager.getLevelPaths()){
        if (saveManager.getLevelData(path).collectedGem)
            ++collectedGems;
    }
    return collectedGems;
}

void LevelSelectState::onResolutionChange(){}