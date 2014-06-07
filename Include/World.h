#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <array>
#include <queue>
#include "ResourceManager.h"
#include "SceneNode.h"
#include "MapData.h"
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"
#include "GameObjectFactory.h"
#include "CollisionHandler.h"
#include "Command.h"
#include "TextNode.h"

class Marvin;

class World{

public:
    World(sf::RenderWindow &window, const std::string &map);
    CommandQueue& getCommandQueue();
    void initialize();
    void update(sf::Time deltaTime);
    void draw();
    void requestReset();
    void requestCompletion();
    bool isComplete();
    float getAttemptTime();
    int getNumDeaths();

    //Querying functions for loading screen
    bool mapLoaded();
    bool objectsLoaded();
    bool texturesLoaded();

private:
    void loadResources();
    void buildScene();
    void spawnPlayer(sf::Vector2f position);
    void renderStaticBodyFixtures(); //For debugging
    void reset();
    void centerPlayerView();

private:
    enum Layer{
        Background,
        Tilemap,
        Object,
        HUD,
        LayerCount
    };
    
private:
    sf::RenderWindow& mWindow;
    sf::View mWorldView;
    CommandQueue mCommandQueue;
    TextureManager mTextureManager;
    FontManager mFontManager;
    TiledJSONLoader mMapLoader;
    Box2DTiledLoader mWorldLoader;
    GameObjectFactory mGameObjectFactory;
    CollisionHandler mCollisionHandler;
    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    Marvin* mPlayerCharacter;
    b2Body* mPlayerBody;
    std::unique_ptr<b2World> mBox2DWorld;
    MapData mMapData;
    std::string mMap;
    sf::Time mLevelTimeElapsed;
    float mCompletionTime;
    TextNode *mTimeText;
    int numDeaths;
    bool mResetRequested;
    bool mCompletionRequested;
    bool mCompleted;
    bool mMapLoaded;
    bool mObjectsLoaded;
    bool mTexturesLoaded;
};