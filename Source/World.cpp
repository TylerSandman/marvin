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
#include "World.h"

World::World(sf::RenderWindow &window, const std::string &map) :
        mWindow(window),
        mWorldView(mWindow.getDefaultView()),
        mTextureManager(TextureManager()),
        mMapLoader(TiledJSONLoader("Resources/Maps/", "Resources/Textures/Tileset/")),
        mWorldLoader(Box2DTiledLoader()),
        mSceneGraph(SceneNode()),
        mPlayer(nullptr){

    mMapLoader.load(map);
    assert(mMapLoader.isMapLoaded());
    mWorldLoader.load(mMapLoader.getTileLayers()[0].tiles);
    assert(mWorldLoader.isWorldLoaded());
    mBox2DWorld = std::unique_ptr<b2World>(mWorldLoader.getWorld());
    mMapData.tileWidth = mMapLoader.getTileSize().x;
    mMapData.tileHeight = mMapLoader.getTileSize().y;
    mMapData.mapWidth = mMapLoader.getMapSize().x;
    mMapData.mapHeight = mMapLoader.getMapSize().y;
    mMapData.tileLayers = mMapLoader.getTileLayers();
    mMapData.objectGroups = mMapLoader.getObjectGroups();
    loadTextures();
    spawnEntities();
    buildScene();


    up = down = left = right = isJumping = false;

    mWorldView.setCenter(sf::Vector2f(512.f,1400.f));
    mWindow.setView(mWorldView);
}

void World::handleInput(sf::Event &event){

    right = (sf::Keyboard::isKeyPressed(sf::Keyboard::D));
    left = (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
    up = (sf::Keyboard::isKeyPressed(sf::Keyboard::W));
    down = (sf::Keyboard::isKeyPressed(sf::Keyboard::S));
}

void World::update(sf::Time deltaTime){

    mBox2DWorld->Step(deltaTime.asSeconds(), 3, 1);

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

    mSceneGraph.update(deltaTime);
  
    //Make sure our view is inside map bounds
    float viewWidth =  mWorldView.getSize().x;
    float viewHeight = mWorldView.getSize().y;
    sf::Vector2f viewCenter = mWorldView.getCenter();
    sf::Vector2f playerPos = mPlayer->getRenderPosition();
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
    mTextureManager.load(TextureID::Background1, "Resources/Textures/Background/grasslands_bg.png");

    //Load our player
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
    sf::Texture &texture = mTextureManager.get(TextureID::Background1);
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture));
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    //Tilemap layer
    std::unique_ptr<TilemapNode> tileMap(new TilemapNode(mMapData));
    mSceneLayers[Tilemap]->attachChild(std::move(tileMap));

    //Object layer (players, enemies, etc)
    mSceneLayers[Object]->attachChild(std::move(std::unique_ptr<Marvin>(mPlayer)));
}

void World::spawnEntities(){

     for(auto &objectGroup : mMapData.objectGroups){
        if (objectGroup.name == "Spawns"){
            for(auto &object : objectGroup.objects){
                if (object.type == "Player"){
                    spawnPlayer(object.position);
                }
            }
        }
    }      
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

    mPlayer = new Marvin(mTextureManager, mPlayerBody);
    mPlayer->setRenderPosition(renderPos);
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