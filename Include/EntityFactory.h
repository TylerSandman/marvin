#pragma once
#include <Box2D\Box2D.h>
#include "ResourceManager.h"
#include "Object.h"
#include "GameObject.h"
#include "Entity.h"
#include "Command.h"
#include "MapData.h"

class EntityFactory{

public:
    EntityFactory(TextureManager &textureManager, MapData data, b2World *world);
    Entity* createEntity(tiled::Object &object);

private:
    b2Body* createPhysicsBody(tiled::Object &object);
    MapData mMapData;
    b2World *mWorld;
    TextureManager &mTextureManager;
};