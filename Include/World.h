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
#include "SoundPlayer.h"
#include "State.h"

class Marvin;

class World{

public:
    World(State::Context context, const std::string &map);
    CommandQueue& getCommandQueue();
    void initialize();
    void update(sf::Time deltaTime);
    void draw();
    void requestReset();
    void requestCompletion();
    bool isComplete();
    float getAttemptTime();
    int getNumDeaths();
    bool isGemCollected();
    void collectGem();
    void onResolutionChange();

    //Querying functions for loading screen
    bool mapLoaded();
    bool objectsLoaded();
    bool texturesLoaded();

private:
    void buildScene();
    void spawnPlayer(sf::Vector2f position);
    void renderStaticBodyFixtures(); //For debugging
    void reset();
    void centerPlayerView();
    void updateSeekers();

private:
    enum Layer{
        Background,
        Tilemap,
        Object,
        HUD,
        Sounds,
        LayerCount
    };
    
private:
    State::Context mContext;
    sf::View mWorldView;
    CommandQueue mCommandQueue;
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
    bool mGemCollected;
    bool mResetRequested;
    bool mCompletionRequested;
    bool mCompleted;
    bool mMapLoaded;
    bool mObjectsLoaded;
    bool mTexturesLoaded;
};