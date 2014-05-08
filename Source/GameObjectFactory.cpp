#include <Box2D\Box2D.h>
#include "MapData.h"
#include "Object.h"
#include "GameObject.h"
#include "Command.h"
#include "GameObjectFactory.h"


GameObjectFactory::GameObjectFactory(){}

GameObjectFactory::GameObjectFactory(MapData data, b2World *world) : 
        mMapData(data), mWorld(world){}

GameObject* GameObjectFactory::createGameObject(tiled::Object &object){

    std::string type = object.type;
    b2Body *objectBody = createPhysicsBody(object);

    GameObject *newObject;
    if (type.compare("Spikes") == 0){
        newObject = new GameObject(
            Category::Type::Damager, object.sprite, objectBody);
    }
    else if (type.compare("Water") == 0){
        newObject = new GameObject(
            Category::Type::Damager, object.sprite, objectBody);
    }
    objectBody->SetUserData(newObject);
    return newObject;
}

b2Body* GameObjectFactory::createPhysicsBody(tiled::Object &object){

    std::string type = object.type;
    sf::FloatRect bounds = object.sprite.getGlobalBounds();
    int mapHeightPixels = mMapData.mapHeight * mMapData.tileHeight;

    //Center our object's origins to play nice with Box2D
    b2Vec2 position = b2Vec2(
        object.position.x + bounds.width/2,
        object.position.y + bounds.height/2);

    //All game objects are static bodies for now
    //TODO handle projectiles
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.allowSleep = true;
    bodyDef.awake = true;

    //Get object size scale relative to a single tile
    //Assumes object is centered on its tile's x-axis
    b2Vec2 objectSize = getObjectSize(object);

    bodyDef.position = b2Vec2(
        position.x / 70.f, 
        (mapHeightPixels - position.y) / 70.f - objectSize.y/2); 
    b2Body *objectBody = mWorld->CreateBody(&bodyDef);

    //A bounding box will be adequate for most, if not all objects
    b2PolygonShape boundingBox;
    boundingBox.SetAsBox(
        bounds.width / 70.f / 2,
        bounds.height / 70.f / 2*objectSize.y);
    b2FixtureDef objectFixture;
    objectFixture.shape = &boundingBox;
    objectFixture.friction = 0.f;
    objectBody->CreateFixture(&objectFixture);
    return objectBody;
}

b2Vec2 GameObjectFactory::getObjectSize(tiled::Object &object){

    std::string type = object.type;

    if (type.compare("Spikes") == 0){
        return b2Vec2(1.f, 0.5f);
    }
    if (type.compare("Water") == 0){
        return b2Vec2(1.f, 0.5f);
    }
    return b2Vec2(1.f, 1.f);
}