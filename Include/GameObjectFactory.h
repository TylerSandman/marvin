#pragma once
#include <Box2D\Box2D.h>
#include "Object.h"
#include "GameObject.h"
#include "Command.h"
#include "MapData.h"

class GameObjectFactory{

public:
    GameObjectFactory();
    GameObjectFactory(MapData data, b2World *world);
    GameObject* createGameObject(tiled::Object &object);

private:
    b2Body* createPhysicsBody(tiled::Object &object);
    b2Vec2 getObjectSize(tiled::Object &object); //Relative to a single tile
    MapData mMapData;
    b2World *mWorld;
};