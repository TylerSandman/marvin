#include <memory>
#include "LevelSelectState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "ContainerButton.h"

LevelSelectState::LevelSelectState(StateStack &stack, Context context) : 
        State(stack, context),
        mLevelPanelView(),
        mGUIContainer(),
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
    mBackdrop.setPosition(0, windowSize.y - backdropTexture.getSize().y);

    //Panel to display levels
    buildLevelPanel();
}

void LevelSelectState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mBackground);
    window.draw(mBackdrop);
    window.draw(mLevelPanel);
    window.setView(mLevelPanelView);
    window.draw(mGUIContainer);
}

bool LevelSelectState::update(sf::Time deltaTime){
    return false;
}

bool LevelSelectState::handleEvent(const sf::Event &event){
    
    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Escape){
            requestStackClear();
            requestStackPush(ID::Menu);
        }
    }
    mGUIContainer.handleEvent(event);
    return false;
}

void LevelSelectState::addLevel(const std::string &name, const std::string &map, bool enabled){
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
            name,
            sf::Color::Black,
            32,
            *getContext().fontManager);
    }
    else{
        levelLabel = std::make_shared<GUI::Label>(
            name,
            sf::Color(0,0,0,100),
            32,
            *getContext().fontManager);
        levelButton->disable();
    }  
    
    levelButton->add(levelLabel);
    mGUIContainer.add(levelButton);
    float posX = windowSize.x*0.5f;
    float posY = 125 + 100*numLevels;
    levelButton->setPosition(posX, posY);

    //Kind of a hacky way to center the text, but this
    //is to compensate for the padding the selection arrow gives
    levelLabel->move(20, -10);

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
    mLevelPanelView.setCenter(mLevelPanel.getPosition());
    mLevelPanelView.setSize(sf::Vector2f(
        static_cast<float>(bgPanelTexture.getSize().x),
        static_cast<float>(bgPanelTexture.getSize().y)));

    //Set viewport ratio according to the ratio of panel size
    //to window size, with a small amount of padding
    mLevelPanelView.setViewport(sf::FloatRect(
        (windowSize.x - bgPanelTexture.getSize().x) / 2.f / windowSize.x,
        ((windowSize.y - bgPanelTexture.getSize().y) / 2.f / windowSize.y) + 0.01f,
        bgPanelTexture.getSize().x / windowSize.x,
        bgPanelTexture.getSize().y / windowSize.y - 0.02f));

    //Level selection buttons
    addLevel("Grasslands", "grasslands.json");
    addLevel("Test Map", "testmap.json");
    addLevel("Waterboy", "waterboy.json", false);
    addLevel("Clear Walk", "clearwalk.json", false);
    addLevel("Go Fast", "gofast.json", false);
    addLevel("Slow Down", "slowdown.json", false);
    addLevel("High Heights", "highheights.json", false);
    addLevel("Hot Pursuit", "hotpursuit.json", false);
    addLevel("Dangah Zone", "dangahzone.json", false);
}