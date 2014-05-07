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
    sf::FloatRect bounds = object.sprite.getGlobalBounds();
    int mapHeightPixels = mMapData.mapHeight * mMapData.tileHeight;

    //Center our object's origins to play nice with Box2D
    b2Vec2 position = b2Vec2(
        object.position.x + bounds.width/2,
        object.position.y + bounds.height/2);

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.allowSleep = true;
    bodyDef.awake = true;

    //Subtract 0.25f from y position since spikes only take up 1/2 of a tile's height
    bodyDef.position = b2Vec2(
        position.x / 70.f, 
        (mapHeightPixels - position.y) / 70.f - 0.25f); 
    b2Body *objectBody = mWorld->CreateBody(&bodyDef);

    b2PolygonShape boundingBox;
    boundingBox.SetAsBox(
        bounds.width / 70.f / 2,
        bounds.height / 70.f / 4); //Divide by 4 since spikes take up half a tile's height
    b2FixtureDef objectFixture;
    objectFixture.shape = &boundingBox;
    objectFixture.friction = 0.f;
    objectBody->CreateFixture(&objectFixture);

    if (type.compare("Spikes") == 0){
        return new GameObject(Category::Type::Damager, object.sprite, objectBody);
    }
    return nullptr;
}