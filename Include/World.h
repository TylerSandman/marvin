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

class Marvin;

class World{

public:
    World(sf::RenderWindow &window, const std::string &map);
    CommandQueue& getCommandQueue();
    void initialize();
    void update(sf::Time deltaTime);
    void draw();
    void requestReset();

    //Querying functions for loading screen
    bool mapLoaded();
    bool objectsLoaded();
    bool texturesLoaded();

private:
    void loadTextures();
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
        LayerCount
    };
    
private:
    sf::RenderWindow& mWindow;
    sf::View mWorldView;
    CommandQueue mCommandQueue;
    TextureManager mTextureManager;
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
    bool mResetRequested;
    bool mMapLoaded;
    bool mObjectsLoaded;
    bool mTexturesLoaded;
};