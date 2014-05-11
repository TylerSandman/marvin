#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <array>
#include <memory>
#include "Marvin.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "TilemapNode.h"
#include "ResourceManager.h"
#include "TiledJSONLoader.h"
#include "Box2DTiledLoader.h"
#include "MapData.h"
#include "GameObjectFactory.h"
#include "CollisionHandler.h"
#include "World.h"


World::World(sf::RenderWindow &window, const std::string &map) :
        mWindow(window),
        mWorldView(mWindow.getDefaultView()),
        mTextureManager(TextureManager()),
        mMapLoader(TiledJSONLoader("Resources/Maps/", "Resources/Textures/Tileset/")),
        mWorldLoader(Box2DTiledLoader()),
        mCollisionHandler(CollisionHandler(*this)),
        mSceneGraph(SceneNode()),
        mPlayerCharacter(nullptr),
        mResetRequested(false){

    mMapLoader.load(map);
    assert(mMapLoader.isMapLoaded());
    mMapData.tileWidth = mMapLoader.getTileSize().x;
    mMapData.tileHeight = mMapLoader.getTileSize().y;
    mMapData.mapWidth = mMapLoader.getMapSize().x;
    mMapData.mapHeight = mMapLoader.getMapSize().y;
    mMapData.tileLayers = mMapLoader.getTileLayers();
    mMapData.objectGroups = mMapLoader.getObjectGroups();

    mWorldLoader.load(mMapLoader.getTileLayers()[0].tiles);
    assert(mWorldLoader.isWorldLoaded());
    mBox2DWorld = std::unique_ptr<b2World>(mWorldLoader.getWorld());
    mBox2DWorld->SetContactListener(&mCollisionHandler);
    mGameObjectFactory = GameObjectFactory(mMapData, mBox2DWorld.get());
    loadTextures();
    buildScene();

    centerPlayerView();
    mWindow.setView(mWorldView);
}

void World::reset(){
    mBox2DWorld.release();
    mPlayerCharacter = nullptr;
    mPlayerBody = nullptr;
    mWorldLoader.load(mMapData.tileLayers[0].tiles);
    assert(mWorldLoader.isWorldLoaded());
    mBox2DWorld = std::unique_ptr<b2World>(mWorldLoader.getWorld());
    mBox2DWorld->SetContactListener(&mCollisionHandler);
    mGameObjectFactory = GameObjectFactory(mMapData, mBox2DWorld.get());
    for (std::size_t i = 0; i < LayerCount; ++i){
        mSceneGraph.detachChild(*mSceneLayers[i]);
    }
    mSceneLayers.empty();
    buildScene();
    centerPlayerView();
    mResetRequested = false;
}

void World::requestReset(){
    mResetRequested = true;
}

CommandQueue& World::getCommandQueue(){
    return mCommandQueue;
}

void World::update(sf::Time deltaTime){

    //It's important we advance our physics engine before updating
    mBox2DWorld->Step(deltaTime.asSeconds(), 6, 2);

    while (!mCommandQueue.empty()){
        mSceneGraph.onCommand(mCommandQueue.front(), deltaTime);
        mCommandQueue.pop();  
    }

    mSceneGraph.update(deltaTime);
    centerPlayerView();

    if (mResetRequested)
        reset();
}

void World::centerPlayerView(){
    //Make sure our view is inside map bounds
    float viewWidth =  mWorldView.getSize().x;
    float viewHeight = mWorldView.getSize().y;
    sf::Vector2f viewCenter = mWorldView.getCenter();
    sf::Vector2f playerPos = mPlayerCharacter->getRenderPosition();
    float mMapWidthPixels = mMapData.mapWidth * 70.f;
    float mMapHeightPixels = mMapData.mapHeight * 70.f;

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
    mWorldView.setCenter(newCenter);
}

void World::draw(){
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::loadTextures(){

    //Load our backgrounds
    mTextureManager.load(TextureID::GrasslandsBackground, "Resources/Textures/Background/grasslands_bg.png");

    //Load our player
    mTextureManager.load(TextureID::PlayerSpriteSheet, "Resources/Textures/Player/player_spritesheet.png");
    mTextureManager.load(TextureID::PlayerStanding, "Resources/Textures/Player/alienGreen_stand.png");
}

void World::buildScene(){

    //Setup our layers
    for (std::size_t i = 0; i < LayerCount; ++i){
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    //Background layer
    sf::Texture &texture = mTextureManager.get(TextureID::GrasslandsBackground);
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    //Tilemap layer
    std::unique_ptr<TilemapNode> tileMap(
        new TilemapNode(mMapData, mWorldLoader.getContours()));
    mSceneLayers[Tilemap]->attachChild(std::move(tileMap));

    //Object layer (players, enemies, etc)

    for(auto &objectGroup : mMapData.objectGroups){
        for(auto &object : objectGroup.objects){
            if (object.type == "Player"){
                spawnPlayer(object.position);
            }
            else{
                mSceneLayers[Object]->attachChild(
                    GameObject::Ptr(mGameObjectFactory.createGameObject(object)));
            }
        }
    } 
    mSceneLayers[Object]->attachChild(std::move(std::unique_ptr<Marvin>(mPlayerCharacter)));
}

void World::spawnPlayer(sf::Vector2f position){

    sf::Sprite playerSprite(mTextureManager.get(TextureID::PlayerStanding));
    sf::FloatRect bounds = playerSprite.getGlobalBounds();
    sf::Vector2f renderPos = position + sf::Vector2f(0.f, bounds.height/2);
    sf::Vector2f centerPlayerPos = sf::Vector2f(
        renderPos.x,
        mMapData.mapHeight*70.f - renderPos.y);

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

    //Special foot sensor to manage jumping/movement
    b2PolygonShape footShape; 
    b2Vec2 footVertices[4];
    footVertices[0] = b2Vec2(-bounds.width / 70.f / 2 + 0.2f, -bounds.height / 70.f / 2 - 0.04f);
    footVertices[1] = b2Vec2(-bounds.width / 70.f / 2 + 0.2f, -bounds.height / 70.f / 2 + 0.05f);
    footVertices[2] = b2Vec2(bounds.width / 70.f / 2 - 0.2f, -bounds.height / 70.f / 2 + 0.05f);
    footVertices[3] = b2Vec2(bounds.width / 70.f / 2 - 0.2f, -bounds.height / 70.f / 2 - 0.04f);
    footShape.Set(footVertices,4);
    b2FixtureDef footFixture;
    footFixture.isSensor = true;
    footFixture.friction = 0.25f;
    footFixture.shape = &footShape;
    mPlayerBody->CreateFixture(&footFixture);

    mPlayerCharacter = new Marvin(mTextureManager, mPlayerBody);
    mPlayerCharacter->setRenderPosition(renderPos);
    mPlayerBody->SetUserData(mPlayerCharacter);
}

void World::renderStaticBodyFixtures(){

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
                    lines.append(sf::Vertex(sf::Vector2f(v1.x * 70.f, (mMapData.mapHeight * 70) - v1.y * 70.f), sf::Color::Blue));
                    lines.append(sf::Vertex(sf::Vector2f(v2.x * 70.f, (mMapData.mapHeight * 70) - v2.y * 70.f), sf::Color::Blue));                
                }
                mWindow.draw(lines);
            }
        }
    }
}