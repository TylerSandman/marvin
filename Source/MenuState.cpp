#include <memory>
#include "MenuState.h"
#include "Container.h"
#include "Image.h"
#include "Button.h"
#include "Constants.h"

MenuState::MenuState(StateStack &stack, Context context) : 
        State(stack, context),
        mGUIContainer(context, context.window->getDefaultView()),
        mGrass(),
        mBackground(),
        mBackdrop(),
        mCharacter(){

    sf::Vector2f windowSize(context.window->getSize());

    //Background
    sf::Texture &groundTexture = context.textureManager->get(TextureID::MenuGround);
    float numRepeats = std::ceil(windowSize.x / groundTexture.getSize().x);
    groundTexture.setRepeated(true);
    mGrass = sf::Sprite(
        groundTexture,
        sf::IntRect(
            0, 0, 
            static_cast<int>(groundTexture.getSize().x * numRepeats * 2), 
            groundTexture.getSize().y));
    float yOffset = mGrass.getGlobalBounds().height;
    mGrass.setPosition(0, windowSize.y - yOffset);

    sf::Texture &bgTexture = context.textureManager->get(TextureID::Background);
    bgTexture.setRepeated(true);
    mBackground.setTexture(bgTexture);
    mBackground.setTextureRect(sf::IntRect(
        0, 0, 
        static_cast<int>(windowSize.x),
        static_cast<int>(windowSize.y)));

    sf::Texture &backdropTexture = context.textureManager->get(TextureID::GrasslandsBackground);
    numRepeats = std::ceil(windowSize.x / backdropTexture.getSize().x);
    backdropTexture.setRepeated(true);
    mBackdrop = sf::Sprite(
        backdropTexture,
        sf::IntRect(
        0, 0,
        static_cast<int>(backdropTexture.getSize().x * numRepeats * 2),
        (backdropTexture.getSize().y)));
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
    mGUIContainer.add(playButton);
    
    auto optionsButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    optionsButton->setCallback(
        [this]() {
            requestStackPush(ID::Option);
        });
    optionsButton->setText("Options", sf::Color::Black);
    mGUIContainer.add(optionsButton);

    auto helpButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    helpButton->setCallback(
        [this]() {
            requestStackPush(ID::Help);
        });
    helpButton->setText("Help", sf::Color::Black);
    mGUIContainer.add(helpButton);

    auto exitButton = std::make_shared<GUI::Button>(
         *getContext().textureManager,
         *getContext().fontManager);
    exitButton->setCallback(
        [this]() {
            exit(0);
        });
    exitButton->setText("Exit", sf::Color::Black);
    mGUIContainer.add(exitButton);

    mGUIContainer.setPosition(windowSize * 0.5f);
    playButton->move(0.f, -90.f);
    optionsButton->move(0.f, -30);
    helpButton->move(0.f, 30);
    exitButton->move(0.f, 90.f);

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

    //Music - check to see if theme is already playing so
    //we don't re-loop it each time we enter the main menu
    if (!context.musicPlayer->isPlaying())
        context.musicPlayer->play(MusicID::MainTheme);
}

void MenuState::draw(){
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    mGUIContainer.setView(getContext().window->getDefaultView());
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
    float test = -MAX_RUN_SPEED * PX_PER_M * deltaTime.asSeconds();
    float test2 = -MAX_RUN_SPEED * 70.f * deltaTime.asSeconds();
    mGrass.move(-MAX_RUN_SPEED * PX_PER_M * deltaTime.asSeconds(), 0.f);

    //Same logic as above for the backdrop
    float repeatBackdropPos = (-1) * mBackdrop.getGlobalBounds().width / 2.f;
    if (mBackdrop.getPosition().x <= repeatBackdropPos){
        mBackdrop.setPosition(0, mBackdrop.getPosition().y);
    }
    mBackdrop.move(-MAX_RUN_SPEED * PX_PER_M * deltaTime.asSeconds(), 0.f);
    mCharacter.update(deltaTime);
    return false;
}

bool MenuState::handleEvent(const sf::Event &event){
    mGUIContainer.handleEvent(event);
    return false;
}

void MenuState::onResolutionChange(){

    sf::Vector2f windowSize(getContext().window->getSize());
    mGUIContainer.setPosition(windowSize * 0.5f);

    sf::Texture &groundTexture = getContext().textureManager->get(TextureID::MenuGround);
    float numRepeats = std::ceil(windowSize.x / groundTexture.getSize().x);
    groundTexture.setRepeated(true);
    mGrass.setTexture(groundTexture);
    mGrass.setTextureRect(
        sf::IntRect(
            0, 0, 
            static_cast<int>(groundTexture.getSize().x * numRepeats * 2), 
            groundTexture.getSize().y));
    float yOffset = mGrass.getGlobalBounds().height;
    mGrass.setPosition(0, windowSize.y - yOffset);

    sf::Texture &bgTexture = getContext().textureManager->get(TextureID::Background);
    bgTexture.setRepeated(true);
    mBackground.setTexture(bgTexture);
    mBackground.setTextureRect(sf::IntRect(
        0, 0, 
        static_cast<int>(windowSize.x),
        static_cast<int>(windowSize.y)));

    sf::Texture &backdropTexture = getContext().textureManager->get(TextureID::GrasslandsBackground);
    numRepeats = std::ceil(windowSize.x / backdropTexture.getSize().x);
    backdropTexture.setRepeated(true);
    mBackdrop.setTexture(backdropTexture);
    mBackdrop.setTextureRect(
        sf::IntRect(
        0, 0,
        static_cast<int>(backdropTexture.getSize().x * numRepeats * 2),
        (backdropTexture.getSize().y)));
    yOffset = mGrass.getGlobalBounds().height + mBackdrop.getGlobalBounds().height;
    mBackdrop.setPosition(0, windowSize.y - yOffset);

    mTitleText.setPosition(
    windowSize.x / 2.f,
    windowSize.y / 4.f);

    yOffset = mGrass.getGlobalBounds().height + mCharacter.getGlobalBounds().height;
    mCharacter.setPosition(windowSize.x / 4.f, windowSize.y - yOffset);
}