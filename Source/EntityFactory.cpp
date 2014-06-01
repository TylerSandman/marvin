#include <Box2D\Box2D.h>
#include "ResourceManager.h"
#include "MapData.h"
#include "Object.h"
#include "Entity.h"
#include "Command.h"
#include "EntityFactory.h"
#include "SnakeSlime.h"
#include "Barnacle.h"

EntityFactory::EntityFactory(TextureManager &textureManager, MapData data, b2World *world) : 
        mTextureManager(textureManager), mMapData(data), mWorld(world){}

Entity* EntityFactory::createEntity(tiled::Object &object){

    std::string type = object.type;
    b2Body *objectBody = createPhysicsBody(object);

    Entity *newEntity;
    sf::Texture &spriteSheet = mTextureManager.get(TextureID::EnemiesSpriteSheet);
    sf::Sprite enemySprite(spriteSheet);
    if (type == "SnakeSlime"){
        enemySprite.setTextureRect(sf::IntRect(424, 187, 53, 147));
    }
    if (type == "Barnacle"){
        enemySprite.setTextureRect(sf::IntRect(318, 239, 51, 57));
    }
    sf::FloatRect bounds = enemySprite.getGlobalBounds();
    sf::Vector2f renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f + 1.f));
    if (type.compare("SnakeSlime") == 0){
        newEntity = new SnakeSlime(
            mTextureManager, objectBody);
    }
    if (type.compare("Barnacle") == 0){
        newEntity = new Barnacle(
            mTextureManager, objectBody);
    }
    newEntity->setRenderPosition(renderPos);
    objectBody->SetUserData(newEntity);
    return newEntity;
}

b2Body* EntityFactory::createPhysicsBody(tiled::Object &object){

    std::string type = object.type;
    int mapHeightPixels = mMapData.mapHeight * mMapData.tileHeight;

    sf::Texture &spriteSheet = mTextureManager.get(TextureID::EnemiesSpriteSheet);
    sf::Sprite enemySprite(spriteSheet);
    if (type == "SnakeSlime"){
        enemySprite.setTextureRect(sf::IntRect(424, 187, 53, 147));
    }
    if (type == "Barnacle"){
        enemySprite.setTextureRect(sf::IntRect(318, 239, 51, 57));
    }
    sf::FloatRect bounds = enemySprite.getGlobalBounds();
    sf::Vector2f renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    sf::Vector2f centerEntityPos = sf::Vector2f(
        renderPos.x,
        mMapData.mapHeight*70.f - renderPos.y);

    //All entities are kinematic bodies for now
    b2BodyDef entityDef;
    entityDef.fixedRotation = true;
    entityDef.type = b2_kinematicBody;
    entityDef.position.Set(centerEntityPos.x / 70.f, centerEntityPos.y / 70.f);
    b2Body *entityBody = mWorld->CreateBody(&entityDef);

    //Bounding box with no friction to prevent sticking to static body walls
    b2PolygonShape boundingBox;
    boundingBox.SetAsBox(bounds.width / 70.f / 2 - 0.1f, bounds.height / 70.f / 2); 
    b2FixtureDef playerFixture;
    playerFixture.friction = 0.f;
    playerFixture.shape = &boundingBox;
    entityBody->CreateFixture(&playerFixture);

    return entityBody;
}