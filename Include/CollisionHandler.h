#pragma once
#include <Box2D\Box2D.h>
#include "SceneNode.h"

class World;

class CollisionHandler : public b2ContactListener{

public:
    CollisionHandler(World &world);
    void BeginContact(b2Contact *contact);
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
    void EndContact(b2Contact *contact);

private:
    bool matchesCategories(SceneNode::Pair &nodes, unsigned int type1, unsigned int type2);

private:
    World &mWorld;
};