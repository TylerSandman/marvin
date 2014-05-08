#pragma once
#include <Box2D\Box2D.h>
#include "SceneNode.h"

class World;

class CollisionHandler : public b2ContactListener{

public:
    CollisionHandler(World &world);
    void BeginContact(b2Contact *contact);

private:
    bool matchesCategories(SceneNode::Pair &nodes, Category::Type type1, Category::Type type2);

private:
    World &mWorld;
};