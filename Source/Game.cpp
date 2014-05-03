#include <SFML/Graphics.hpp>
#include <array>
#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"
#include "ResourceManager.h"
#include "Game.h"

sf::Time FRAME_RATE = sf::seconds(1.f/60.f);
//float jumpVelocity = sqrt(fabs(2 * gravity * MAX_JUMP_HEIGHT));
//float jumpTerminationVelocity = sqrt(powf(jumpVelocity,2) + 2 * gravity * (MAX_JUMP_HEIGHT - MIN_JUMP_HEIGHT));

Game::Game(const std::string &map) : 
        mMapLoader(TiledJSONLoader("Resources/Maps/", "Resources/Textures/Tileset/")),
        mWorldLoader(Box2DTiledLoader()),
        mWindow(sf::RenderWindow(sf::VideoMode(1024,768), "Marvin", sf::Style::Close)),
        mView(mWindow.getDefaultView()){
        
    mMapLoader.load(map);
    assert(mMapLoader.isMapLoaded());
    mWorldLoader.load(mMapLoader.getTileLayers()[0].tiles);
    assert(mWorldLoader.isWorldLoaded());
    mBox2DWorld = std::unique_ptr<b2World>(mWorldLoader.getWorld());

    mTileWidth = mMapLoader.getTileSize().x;
    mTileHeight = mMapLoader.getTileSize().y;
    mMapWidth = mMapLoader.getMapSize().x;
    mMapHeight = mMapLoader.getMapSize().y;
    mTileLayers = mMapLoader.getTileLayers();
    mObjectGroups = mMapLoader.getObjectGroups();
    mTimeJumpHeld = 0;
    isJumping = false;

    loadTextures();
    buildScene();
    spawnEntities();

    mView.setCenter(sf::Vector2f(512.f,1400.f));
    mWindow.setView(mView);
    mWindow.setVerticalSyncEnabled(true);
    up = down = left = right = false;
}

void Game::run(){

    int velocityIterations = 6;
    int positionIterations = 2;

    sf::Time elapsedTime = sf::Time::Zero;
    sf::Clock clock;
    while (mWindow.isOpen()){
        elapsedTime += clock.restart();
        while (elapsedTime > FRAME_RATE){
            elapsedTime -= FRAME_RATE;
            handleInput();
            mBox2DWorld->Step(1.f/60.f, velocityIterations, positionIterations);
            update(FRAME_RATE);
        }
        render();
    }
}

void Game::handleInput(){

    sf::Event event;
    while (mWindow.pollEvent(event)){
        if (event.type == sf::Event::Closed){
            mWindow.close();
        }
        right = (sf::Keyboard::isKeyPressed(sf::Keyboard::D));
        left = (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        up = (sf::Keyboard::isKeyPressed(sf::Keyboard::W));
        down = (sf::Keyboard::isKeyPressed(sf::Keyboard::S));
    }
}
void Game::update(sf::Time deltaTime){

    float gravity = mBox2DWorld->GetGravity().y;
    float jumpVelocity = sqrt(fabs(2 * gravity * MAX_JUMP_HEIGHT));
    float jumpTerminationVelocity = sqrt(powf(jumpVelocity,2) + 2 * gravity * (MAX_JUMP_HEIGHT - MIN_JUMP_HEIGHT));
    b2Vec2 curVelocity = mPlayerBody->GetLinearVelocity();

    if (right){
        b2Vec2 interpolatedVelocity = b2Vec2(
            ACCELERATION_CONSTANT * MAX_RUN_SPEED + (1 - ACCELERATION_CONSTANT) * curVelocity.x,
            curVelocity.y);
        mPlayerBody->SetLinearVelocity(b2Vec2(interpolatedVelocity));
    }
    else if (left){
        b2Vec2 interpolatedVelocity = b2Vec2(
            ACCELERATION_CONSTANT * -MAX_RUN_SPEED + (1 - ACCELERATION_CONSTANT) * curVelocity.x,
            curVelocity.y);
        mPlayerBody->SetLinearVelocity(b2Vec2(interpolatedVelocity));
    }
    else{
        b2Vec2 interpolatedVelocity = b2Vec2(
            DECELERATION_CONSTANT * curVelocity.x,
            curVelocity.y);
        mPlayerBody->SetLinearVelocity(b2Vec2(interpolatedVelocity));
    }

    curVelocity = mPlayerBody->GetLinearVelocity();
    isJumping = (curVelocity.y == 0 ? false : true);
    if (up){
        if (!isJumping){
            mPlayerBody->SetLinearVelocity(
            b2Vec2(curVelocity.x, jumpVelocity));
            isJumping = true;
        }
    }
    else{
        mPlayerBody->SetLinearVelocity(
        b2Vec2(curVelocity.x, std::min(jumpTerminationVelocity, curVelocity.y)));
    }

    b2Vec2 physPos = mPlayerBody->GetPosition();
    sf::Vector2f renderPos = sf::Vector2f(physPos.x * 70.f, mMapHeight * 70.f - physPos.y * 70.f);
    mPlayer.setPosition(renderPos);
  
    //Make sure our view is inside map bounds
    float viewWidth =  mView.getSize().x;
    float viewHeight = mView.getSize().y;
    sf::Vector2f viewCenter = mView.getCenter();
    sf::Vector2f playerPos = mPlayer.getPosition();
    float mMapWidthPixels = mMapWidth * 70.f;
    float mMapHeightPixels = mMapHeight * 70.f;

    float xOffset = 0;
    float yOffset = 0;

    if (playerPos.y - viewHeight/2 < 0)
        yOffset = fabs(playerPos.y - viewHeight/2);
    else if (playerPos.y + viewHeight/2 > mMapHeightPixels)
        yOffset = -1 * (viewHeight/2 - (mMapHeightPixels - playerPos.y));
    if (playerPos.x - viewWidth/2 < 0)
        xOffset = fabs(playerPos.x - viewWidth/2);
    else if (playerPos.x + viewWidth/2 > mMapWidthPixels)
        xOffset = -1 * (viewWidth/2 - (mMapWidthPixels - playerPos.x));

    sf::Vector2f newCenter(playerPos.x + xOffset, playerPos.y + yOffset);
    mWindow.clear();
    mView.setCenter(newCenter);
    mWindow.setView(mView);
    

}

void Game::render(){

    mWindow.clear();
    renderBackground();
    renderTileLayers();
    renderObjectGroups();
    renderEntities();
    //renderStaticBodyFixtures(); //For debugging
    mWindow.display();
}

void Game::renderBackground(){
    mWindow.draw(mBackground);
}

void Game::renderTileLayers(){

    //Culling logic to only render visible tiles
    sf::Vector2f center = mView.getCenter();
    sf::Vector2f size = mView.getSize();
    int viewStartCullX = static_cast<int>
        (std::floor((center.x - (size.x/2))/mTileWidth)) - 1;
    int viewStartCullY = static_cast<int>
        (std::floor((center.y - (size.y/2))/mTileHeight)) - 1;
    int viewEndCullX = static_cast<int>
        (std::ceil((center.x + (size.x/2))/mTileWidth)) + 1;
    int viewEndCullY = static_cast<int>
        (std::ceil((center.y + (size.y/2))/mTileHeight)) + 1;

    if (viewStartCullX < 0) 
        viewStartCullX = 0;
    if (viewStartCullX > mMapWidth)
        viewStartCullX = mMapWidth;
    if (viewStartCullY < 0)
        viewStartCullY = 0;
    if (viewStartCullY > mMapHeight)
        viewStartCullY = mMapHeight;
    if (viewEndCullX < 0) 
        viewEndCullX = 0;
    if (viewEndCullX > mMapWidth)
        viewEndCullX = mMapWidth;
    if (viewEndCullY < 0)
        viewEndCullY = 0;
    if (viewEndCullY > mMapHeight)
        viewEndCullY = mMapHeight;

    for (int i = viewStartCullY; i < viewEndCullY; ++i){
        for (int j = viewStartCullX; j < viewEndCullX; ++j){
            for(auto &layer : mTileLayers){
                tiled::Tile &tile = layer.tiles[i][j];
                if (tile.sprite.getTexture()) mWindow.draw(tile.sprite);
            }
        }
    }
}

void Game::renderObjectGroups(){

    //Culling logic only needed when we have a large amount of tiled objects
    for(auto &group : mObjectGroups){
        for(auto &object : group.objects){
            mWindow.draw(object.sprite);
        }
    }
}

void Game::renderEntities(){
   mWindow.draw(mPlayer);
}

void Game::renderStaticBodyFixtures(){

    for (b2Body *b = mBox2DWorld->GetBodyList(); b; b = b->GetNext()){

        if (b->GetType() == b2_staticBody){
            for (b2Fixture *fixture = b->GetFixtureList(); fixture; fixture = fixture->GetNext()){

                //We know it's a chain shape
                b2ChainShape *chain = static_cast<b2ChainShape*>(fixture->GetShape());
                sf::VertexArray lines(sf::PrimitiveType::Lines);

                for (int i = 0; i < chain->GetChildCount(); ++i){

                    b2EdgeShape edge;
                    chain->GetChildEdge(&edge, i);
                    b2Vec2 v1 = edge.m_vertex1;
                    b2Vec2 v2 = edge.m_vertex2;   
                    lines.append(sf::Vertex(sf::Vector2f(v1.x * 70.f, (mMapHeight * 70) - v1.y * 70.f), sf::Color::Blue));
                    lines.append(sf::Vertex(sf::Vector2f(v2.x * 70.f, (mMapHeight * 70) - v2.y * 70.f), sf::Color::Blue));                
                }
                mWindow.draw(lines);
            }
        }
    }
}

void Game::loadTextures(){

    //Load our backgrounds
    mTextureManager.load(TextureID::Background1, "Resources/Textures/Background/grasslands_bg.png");

    //Load our player
    mTextureManager.load(TextureID::PlayerStanding, "Resources/Textures/Player/alienGreen_stand.png");
}

void Game::buildScene(){

    mBackground.setTexture(mTextureManager.get(TextureID::Background1));
    mPlayer.setTexture(mTextureManager.get(TextureID::PlayerStanding));
}

void Game::spawnEntities(){

    for(auto &objectGroup : mObjectGroups){
        if (objectGroup.name == "Spawns"){
            for(auto &object : objectGroup.objects){
                if (object.type == "Player"){
                    spawnPlayer(object.position);
                }
            }
        }
    }       
}

void Game::spawnPlayer(sf::Vector2f position){

    sf::FloatRect bounds = mPlayer.getGlobalBounds();
    mPlayer.setPosition(position + sf::Vector2f(0.f, bounds.height));
    mPlayer.setOrigin(bounds.width/2, bounds.height/2);
    sf::Vector2f centerPlayerPos = sf::Vector2f(mPlayer.getPosition().x + bounds.width/2, mMapHeight*70.f - mPlayer.getPosition().y + bounds.height/2);

    //Create our box2D body
    b2BodyDef playerDef;
    playerDef.fixedRotation = true;
    playerDef.type = b2_dynamicBody;
    playerDef.position.Set(centerPlayerPos.x / 70.f, centerPlayerPos.y / 70.f);
    mPlayerBody = mBox2DWorld->CreateBody(&playerDef);

    //Bounding box with no friction to prevent sticking to static body walls
    b2PolygonShape boundingBox;
    boundingBox.SetAsBox(bounds.width / 70.f / 2 - 0.1f, bounds.height / 70.f / 2); 
    b2FixtureDef playerFixture;
    playerFixture.friction = 0.f;
    playerFixture.shape = &boundingBox;
    mPlayerBody->CreateFixture(&playerFixture);

    //Special foot fixture to simulate friction on the ground
    b2PolygonShape footShape; 
    b2Vec2 footVertices[4];
    footVertices[0] = b2Vec2(-bounds.width / 70.f / 2 + 0.15f, -bounds.height / 70.f / 2);
    footVertices[1] = b2Vec2(-bounds.width / 70.f / 2 + 0.15f, -bounds.height / 70.f / 2 + 0.05f);
    footVertices[2] = b2Vec2(bounds.width / 70.f / 2 - 0.15f, -bounds.height / 70.f / 2 + 0.05f);
    footVertices[3] = b2Vec2(bounds.width / 70.f / 2 - 0.15f, -bounds.height / 70.f / 2);
    footShape.Set(footVertices,4);
    b2FixtureDef footFixture;
    footFixture.friction = 0.25f;
    footFixture.shape = &footShape;
    mPlayerBody->CreateFixture(&footFixture);
}