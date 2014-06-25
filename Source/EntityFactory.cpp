#include <Box2D\Box2D.h>
#include <string>
#include <limits>
#include <json_spirit_reader_template.h>
#include "ResourceManager.h"
#include "MapData.h"
#include "Object.h"
#include "Entity.h"
#include "Command.h"
#include "EntityFactory.h"
#include "SnakeSlime.h"
#include "Barnacle.h"
#include "Slime.h"
#include "GrassBlock.h"
#include "GrassPlatform.h"
#include "Bee.h"
#include "BossSlime.h"

EntityFactory::EntityFactory(TextureManager &textureManager, MapData data, b2World *world) : 
        mTextureManager(textureManager), mMapData(data), mWorld(world){}

Entity* EntityFactory::createEntity(tiled::Object &object){

    std::string type = object.type;
    b2Body *objectBody = createPhysicsBody(object);

    Entity *newEntity;
    sf::Texture &spriteSheet = mTextureManager.get(TextureID::EnemiesSpriteSheet);
    sf::Texture &bossSlime = mTextureManager.get(TextureID::Boss);

    sf::Sprite enemySprite;

    if (type == "BossSlime"){
        enemySprite.setTexture(bossSlime);
        enemySprite.setTextureRect(sf::IntRect(0, 214, 295, 186));
    }
    else
        enemySprite.setTexture(spriteSheet);

    if (type == "SnakeSlime"){
        enemySprite.setTextureRect(sf::IntRect(424, 187, 53, 147));
    }
    if (type == "Barnacle"){
        enemySprite.setTextureRect(sf::IntRect(318, 239, 51, 57));
    }
    if (type == "Slime"){
        enemySprite.setTextureRect(sf::IntRect(140, 65, 49, 34));
    }
    if (type == "GrassBlock"){
        enemySprite.setTextureRect(sf::IntRect(0, 141, 71, 70));
    }
    if (type == "GrassPlatform"){
        enemySprite.setTextureRect(sf::IntRect(579, 272, 209, 39));
    }
    if (type == "GrassPlatformLarge"){
        enemySprite.setTextureRect(sf::IntRect(210, 0, 585, 39));
    }
    if (type == "Bee"){
        enemySprite.setTextureRect(sf::IntRect(315, 353, 56, 48));
    }
    sf::FloatRect bounds = enemySprite.getGlobalBounds();
    sf::Vector2f renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f + 1.f));
    if (type.compare("SnakeSlime") == 0){
        newEntity = new SnakeSlime(
            mTextureManager, objectBody);
    }
    if (type.compare("Barnacle") == 0){
        
        bool flipped = false;
        if (object.properties.find("flipped") != object.properties.end()){
            std::string flippedJSON = object.properties["flipped"].get_str();
            json_spirit::mValue flippedValue;
            json_spirit::read_string(flippedJSON, flippedValue);
            flipped = flippedValue.get_bool();
            if (flipped)
                renderPos = renderPos - sf::Vector2f(0.f, (mMapData.tileHeight - bounds.height + 1.f));
        }        
        newEntity = new Barnacle(
            mTextureManager, objectBody, flipped);
    }
    if (type.compare("Slime") == 0){
        newEntity = new Slime(
            mTextureManager, objectBody, stof(object.properties["jumpHeight"].get_str()));
    }
    if (type.compare("GrassBlock") == 0){
        newEntity = new GrassBlock(
            mTextureManager, objectBody, stof(object.properties["jumpHeight"].get_str()));
    }
    if (type.compare("GrassPlatform") == 0 || type.compare("GrassPlatformLarge") == 0){

        //Parse waypoints
        std::vector<sf::Vector2f> waypoints;
        std::string waypointsJSON = object.properties["waypoints"].get_str();
        json_spirit::mValue waypointsValue;
        json_spirit::read_string(waypointsJSON, waypointsValue);
        json_spirit::mArray waypointsArray = waypointsValue.get_array();
        for (auto &waypoint : waypointsArray){
            json_spirit::mObject waypointObj = waypoint.get_obj();
            sf::Vector2f waypoint(
                waypointObj["x"].get_real() * 70.f,
                waypointObj["y"].get_real() * 70.f);
            waypoints.push_back(waypoint);
        }
        if (type.compare("GrassPlatform") == 0)
            newEntity = new GrassPlatform(
                mTextureManager, objectBody, stof(object.properties["velocity"].get_str()), waypoints);
        else
            newEntity = new GrassPlatform(
                mTextureManager, objectBody, stof(object.properties["velocity"].get_str()), waypoints, true);

    }
    if (type.compare("Bee") == 0){

        //Parse waypoints
        std::vector<sf::Vector2f> waypoints;
        if (object.properties.find("waypoints") != object.properties.end()){
            std::string waypointsJSON = object.properties["waypoints"].get_str();
            json_spirit::mValue waypointsValue;
            json_spirit::read_string(waypointsJSON, waypointsValue);
            json_spirit::mArray waypointsArray = waypointsValue.get_array();
            for (auto &waypoint : waypointsArray){
                json_spirit::mObject waypointObj = waypoint.get_obj();
                sf::Vector2f waypoint(
                    waypointObj["x"].get_real() * 70.f,
                    waypointObj["y"].get_real() * 70.f);
                waypoints.push_back(waypoint);
            }
        }
        if (object.properties.find("steering") != object.properties.end())
            newEntity = new Bee(
                mTextureManager, objectBody, stof(object.properties["velocity"].get_str()), waypoints, stof(object.properties["steering"].get_str()));
        
        else
            newEntity = new Bee(
                mTextureManager, objectBody, stof(object.properties["velocity"].get_str()), waypoints);
    }

    if (type.compare("BossSlime") == 0){
        newEntity = new BossSlime(
            mTextureManager, objectBody, stof(object.properties["velocity"].get_str()));
    }

    newEntity->setRenderPosition(renderPos);
    objectBody->SetUserData(newEntity);
    return newEntity;
}

b2Body* EntityFactory::createPhysicsBody(tiled::Object &object){

    std::string type = object.type;
    int mapHeightPixels = mMapData.mapHeight * mMapData.tileHeight;

    sf::Texture &spriteSheet = mTextureManager.get(TextureID::EnemiesSpriteSheet);
    sf::Texture &bossSlime = mTextureManager.get(TextureID::Boss);

    sf::Sprite enemySprite(spriteSheet);
    sf::FloatRect bounds;
    sf::Vector2f renderPos;

    if (type == "BossSlime"){
        enemySprite.setTexture(bossSlime);
        enemySprite.setTextureRect(sf::IntRect(0, 214, 295, 186));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    }
    if (type == "SnakeSlime"){
        enemySprite.setTextureRect(sf::IntRect(424, 187, 53, 147));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    }
    if (type == "Barnacle"){
        bool flipped = false;
        if (object.properties.find("flipped") != object.properties.end()){
            std::string flippedJSON = object.properties["flipped"].get_str();
            json_spirit::mValue flippedValue;
            json_spirit::read_string(flippedJSON, flippedValue);
            flipped = flippedValue.get_bool();
        }
        enemySprite.setTextureRect(sf::IntRect(318, 239, 51, 57));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
        if (flipped){
            enemySprite.move(0.f, - (mMapData.tileHeight - enemySprite.getGlobalBounds().height));
            renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f , 0.f);
        }       
    }
    if (type == "Slime"){
        enemySprite.setTextureRect(sf::IntRect(140, 65, 49, 34));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    }
    if (type == "GrassBlock"){
        enemySprite.setTextureRect(sf::IntRect(0, 141, 71, 70));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    }
    if (type == "GrassPlatform"){       
        enemySprite.setTextureRect(sf::IntRect(579, 272, 209, 39));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));      
    }
    if (type == "GrassPlatformLarge"){
        enemySprite.setTextureRect(sf::IntRect(210, 0, 585, 39));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));   
    }
    if (type == "Bee"){
        enemySprite.setTextureRect(sf::IntRect(315, 353, 56, 48));
        bounds = enemySprite.getGlobalBounds();
        renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    }
    sf::Vector2f centerEntityPos = sf::Vector2f(
        renderPos.x,
        mMapData.mapHeight*70.f - renderPos.y);

    b2BodyDef entityDef;
    entityDef.fixedRotation = true;
    if (type == "GrassBlock" || type == "Slime")
        entityDef.type = b2_dynamicBody;
    else
        entityDef.type = b2_kinematicBody;
    entityDef.position.Set(centerEntityPos.x / 70.f, centerEntityPos.y / 70.f);
    b2Body *entityBody = mWorld->CreateBody(&entityDef);

    //Bounding box with no friction to prevent sticking to static body walls
    b2PolygonShape boundingBox;
    boundingBox.SetAsBox(bounds.width / 70.f / 2 - 0.1f, bounds.height / 70.f / 2); 
    b2FixtureDef playerFixture;
    playerFixture.friction = 1.f;
    playerFixture.restitution = 0.f;
    playerFixture.shape = &boundingBox;
    entityBody->CreateFixture(&playerFixture);

    //Bottom sensor for GrassBlock
    if (type == "GrassBlock"){
        b2PolygonShape blockSensor; 
        b2Vec2 footVertices[4];
        footVertices[0] = b2Vec2(-bounds.width / 70.f / 2 + 0.2f, -bounds.height / 70.f / 2 - 0.01f);
        footVertices[1] = b2Vec2(-bounds.width / 70.f / 2 + 0.2f, -bounds.height / 70.f / 2 + 0.05f);
        footVertices[2] = b2Vec2(bounds.width / 70.f / 2 - 0.2f, -bounds.height / 70.f / 2 + 0.05f);
        footVertices[3] = b2Vec2(bounds.width / 70.f / 2 - 0.2f, -bounds.height / 70.f / 2 - 0.04f);
        blockSensor.Set(footVertices,4);
        b2FixtureDef footFixture;
        footFixture.isSensor = true;
        footFixture.friction = 0.25f;
        footFixture.shape = &blockSensor;
        entityBody->CreateFixture(&footFixture);
        b2MassData grassMassData;
        entityBody->GetMassData(&grassMassData);
        grassMassData.mass = static_cast<float>(UINT_MAX);
        entityBody->SetMassData(&grassMassData);
    }
    return entityBody;
}