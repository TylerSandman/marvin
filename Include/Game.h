#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"
#include "ResourceManager.h"

class Game{

public:
    Game(const std::string &map);
    void run();

private:
    void handleInput();
    void update(sf::Time deltaTime);
    void render();
    void loadTextures();
    void buildScene();
    void spawnEntities();
    void spawnPlayer(sf::Vector2f position);
    void renderBackground();
    void renderTileLayers();
    void renderObjectGroups();
    void renderEntities();
    void renderStaticBodyFixtures(); //For physics debugging

private:
    TiledJSONLoader mMapLoader;
    Box2DTiledLoader mWorldLoader;
    TextureManager mTextureManager;
    std::unique_ptr<b2World> mBox2DWorld;
    sf::RenderWindow mWindow;
    sf::View mView;
    int mTileWidth;
    int mTileHeight;
    int mMapWidth;
    int mMapHeight;
    float mTimeJumpHeld;
    bool isJumping;
    tiled::TileLayers mTileLayers;
    tiled::ObjectGroups mObjectGroups;
    sf::Sprite mBackground;
    sf::Sprite mPlayer;
    b2Body *mPlayerBody;
    bool up, down, left, right;
};