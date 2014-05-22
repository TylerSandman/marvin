#include <memory>
#include "MenuState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"

MenuState::MenuState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context.window->getDefaultView()),
        mGrass(),
        mBackground(){

    sf::Vector2f windowSize(context.window->getSize());

    //Background
    sf::Texture &groundTexture = context.textureManager->get(TextureID::MenuGround);
    int numRepeats = std::ceil(windowSize.x / groundTexture.getSize().x);
    groundTexture.setRepeated(true);
    mGrass = sf::Sprite(
        groundTexture,
        sf::IntRect(
            0, 0, 
            groundTexture.getSize().x * numRepeats * 2, 
            groundTexture.getSize().y));
    int yOffset = mGrass.getGlobalBounds().height;
    mGrass.setPosition(0, windowSize.y - yOffset);

    sf::Texture &bgTexture = context.textureManager->get(TextureID::Background);
    bgTexture.setRepeated(true);
    mBackground.setTexture(bgTexture);
    mBackground.setTextureRect(sf::IntRect(0, 0, windowSize.x, windowSize.y));

    sf::Texture &backdropTexture = context.textureManager->get(TextureID::GrasslandsBackground);
    numRepeats = std::ceil(windowSize.x / backdropTexture.getSize().x);
    backdropTexture.setRepeated(true);
    mBackdrop = sf::Sprite(
        backdropTexture,
        sf::IntRect(
        0, 0,
        backdropTexture.getSize().x * numRepeats * 2,
        backdropTexture.getSize().y));
    yOffset = mGrass.getGlobalBounds().height + mBackdrop.getGlobalBounds().height;
    mBackdrop.setPosition(0, windowSize.y - yOffset);

    //Buttons
    auto playButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    playButton->setCallback(
        [this]() {
            requestStackPop();
            requestStackPush(ID::LevelSelect);
        });
    playButton->setText("Play", sf::Color::Black);
    playButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f - 70));
    mGUIContainer.add(playButton);
    
    auto optionsButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    optionsButton->setCallback(
        [this]() {
            requestStackClear();
            requestStackPush(ID::LevelSelect);
        });
    optionsButton->setText("Options", sf::Color::Black);
    optionsButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f));
    mGUIContainer.add(optionsButton);

    auto exitButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    exitButton->setCallback(
        [this]() {
            exit(0);
        });
    exitButton->setText("Exit", sf::Color::Black);
    exitButton->setPosition(
        sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f + 70));
    mGUIContainer.add(exitButton);

    //Player Animation
    sf::Texture &spriteSheet = context.textureManager->get(TextureID::PlayerSpriteSheet);
    
    mCharacterAnimation.setSpriteSheet(spriteSheet);
    mCharacterAnimation.addFrame(sf::IntRect(0, 0, 70, 100));
    mCharacterAnimation.addFrame(sf::IntRect(70, 0, 70, 100));
    mCharacterAnimation.addFrame(sf::IntRect(140, 0, 70, 100));

    mCharacter = AnimatedSprite(sf::seconds(0.1f));
    mCharacter.setAnimation(mCharacterAnimation);
    yOffset = mGrass.getGlobalBounds().height + mCharacter.getGlobalBounds().height;
    mCharacter.setPosition(windowSize.x / 4.f, windowSize.y - yOffset);
    mCharacter.setScale(-1.f, 1.f);
    mCharacter.play();

    //Title
    sf::Font &font = context.fontManager->get(FontID::Main);
    mTitleText.setFont(font);
    mTitleText.setCharacterSize(60);
    mTitleText.setString("Marvin");
    mTitleText.setColor(sf::Color(111,196,169,255));
    mTitleText.setPosition(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 4.f);
    mTitleText.setOrigin(
        mTitleText.getGlobalBounds().width / 2.f,
        mTitleText.getGlobalBounds().height / 2.f);
}

void MenuState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mBackground);
    window.draw(mBackdrop);
    window.draw(mGrass);
    window.draw(mCharacter);
    window.draw(mTitleText);
    window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time deltaTime){
    sf::RenderWindow &window = *getContext().window;
    sf::Vector2u windowSize = window.getSize();

    //When the ground hits this position loop it back to
    //where it started to simulate an infinite scroll   
    float repeatGroundPos = (-1) * mGrass.getGlobalBounds().width / 2.f;
    if (mGrass.getPosition().x <= repeatGroundPos){
        mGrass.setPosition(0, mGrass.getPosition().y);
    }
    mGrass.move(-MAX_RUN_SPEED * 70.f * deltaTime.asSeconds(), 0.f);

    //Same logic as above for the backdrop
    float repeatBackdropPos = (-1) * mBackdrop.getGlobalBounds().width / 2.f;
    if (mBackdrop.getPosition().x <= repeatBackdropPos){
        mBackdrop.setPosition(0, mBackdrop.getPosition().y);
    }
    mBackdrop.move(-MAX_RUN_SPEED * 70.f * deltaTime.asSeconds(), 0.f);
    mCharacter.update(deltaTime);
    return false;
}

bool MenuState::handleEvent(const sf::Event &event){
    mGUIContainer.handleEvent(event);
    return false;
}