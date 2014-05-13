#include "LevelLoadingState.h"

LevelLoadingState::LevelLoadingState(StateStack &stack, Context context, const std::string &map) : 
        State(stack, context),
        mPlayer(*context.player),
        mLoadingThread([context, map](){ context.levelManager->load(map); }),
        mMutex(),
        mMap(map),
        mMapLoaded(false),
        mObjectsLoaded(false),
        mTexturesLoaded(false){

    sf::Font &font = context.fontManager->get(FontID::Main);
    mLoadingText.setFont(font);
    setLoadingText("Loading Map");
    mLoadingText.setPosition(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);
    context.levelManager->insert(mMap);
    mWorld = context.levelManager->get(mMap);
    mLoadingThread.launch();   
}

void LevelLoadingState::draw(){

    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(mLoadingText);
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