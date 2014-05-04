#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <array>
#include "ResourceManager.h"
#include "SceneNode.h"
#include "MapData.h"
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"

class Marvin;

class World{

public:
    World(sf::RenderWindow &window, const std::string &map);
    void handleInput(sf::Event &event);
    void update(sf::Time deltaTime);
    void draw();

private:
    void loadTextures();
    void buildScene();
    void spawnEntities();
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
    TextureManager mTextureManager;
    TiledJSONLoader mMapLoader;
    Box2DTiledLoader mWorldLoader;
    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    Marvin* mPlayer;
    b2Body* mPlayerBody;
    std::unique_ptr<b2World> mBox2DWorld;
    MapData mMapData;
    bool up, down, left, right, isJumping;
};