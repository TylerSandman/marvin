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
    MapData mMapData;
    b2World *mWorld;
};