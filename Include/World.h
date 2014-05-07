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
#include "Command.h"

class Marvin;

class World{

public:
    World(sf::RenderWindow &window, const std::string &map);
    CommandQueue& getCommandQueue();
    void update(sf::Time deltaTime);
    void draw();

private:
    void loadTextures();
    void buildScene();
    void spawnPlayer(sf::Vector2f position);
    void renderStaticBodyFixtures(); //For debugging

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
    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    Marvin* mPlayerCharacter;
    b2Body* mPlayerBody;
    std::unique_ptr<b2World> mBox2DWorld;
    MapData mMapData;
};