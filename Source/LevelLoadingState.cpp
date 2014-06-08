#include "LevelLoadingState.h"
#include "AnimatedSprite.h"

LevelLoadingState::LevelLoadingState(StateStack &stack, Context context, const std::string &map) : 
        State(stack, context),
        mPlayer(*context.player),
        mLoadingThread([context, map](){ context.levelManager->load(map); }),
        mMutex(),
        mAnimation(),
        mSprite(),
        mMap(map),
        mMapLoaded(false),
        mObjectsLoaded(false),
        mTexturesLoaded(false){

    context.textureManager->load(TextureID::SpinnerSpriteSheet, "Resources/Textures/Enemy/spinner_spritesheet.png");
    sf::Texture &bgTexture = context.textureManager->get(TextureID::CastleBackground);
    sf::Texture &spriteSheet = context.textureManager->get(TextureID::SpinnerSpriteSheet);
    bgTexture.setRepeated(true);
    mBackground = sf::Sprite(
        bgTexture,
        sf::IntRect(0, 0, context.window->getSize().x, context.window->getSize().y));
    
    mAnimation.setSpriteSheet(spriteSheet);
    mAnimation.addFrame(sf::IntRect(0, 0, 65, 65));
    mAnimation.addFrame(sf::IntRect(65, 0, 65, 65));
    mSprite = AnimatedSprite(sf::seconds(0.1f));
    mSprite.setAnimation(mAnimation);
    mSprite.play();
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mSprite.setPosition(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f - 50.f);
    sf::Font &font = context.fontManager->get(FontID::Main);
    mLoadingText.setFont(font);
    setLoadingText("Loading Map");
    mLoadingText.setPosition(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);
    context.levelManager->insert(mMap);
    mWorld = context.levelManager->get(mMap);
    mLoadingThread.launch();   

    //Stop music while loading
    context.musicPlayer->stop();
}

void LevelLoadingState::draw(){

    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mBackground);
    window.draw(mLoadingText);
    window.draw(mSprite);
}

bool LevelLoadingState::update(sf::Time deltaTime){

    if (!mMapLoaded){
        mMapLoaded = mWorld->mapLoaded();
        if (mMapLoaded){
            setLoadingText("Loading Level Objects");
        }
    }
    if (!mObjectsLoaded){
        mObjectsLoaded = mWorld->objectsLoaded();
        if (mObjectsLoaded){
            setLoadingText("Loading Textures");
        }
    }
    if (!mTexturesLoaded){
        mTexturesLoaded = mWorld->texturesLoaded();
        if (mTexturesLoaded){
            setLoadingText("Initializing");
        }
    }
    if (levelLoaded()){
        requestStackPop();
        requestStackPush(State::ID::Play, mMap);
    }
    mSprite.update(deltaTime);
    return false;  
}

bool LevelLoadingState::handleEvent(const sf::Event &event){
    return false;
}

bool LevelLoadingState::levelLoaded(){
    sf::Lock lock(mMutex);
    return (mMapLoaded && mObjectsLoaded && mTexturesLoaded);
}

void LevelLoadingState::setLoadingText(const std::string &text){
    mLoadingText.setString(text);
    mLoadingText.setOrigin(
        mLoadingText.getGlobalBounds().width / 2.f,
        mLoadingText.getGlobalBounds().height / 2.f);
}