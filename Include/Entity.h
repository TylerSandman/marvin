#pragma once
#include <Box2D\Box2D.h>
#include "SceneNode.h"

class Entity : public SceneNode{

public:
    void setVelocity(b2Vec2 velocity);
    void setVelocity(float x, float y);
    b2Vec2 getVelocity();

private:
    b2Vec2 mVelocity;
};