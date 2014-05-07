#pragma once
#include <Box2D\Box2D.h>
#include "SceneNode.h"

class Entity : public SceneNode{

public:
    virtual void setVelocity(b2Vec2 velocity);
    virtual void setVelocity(float x, float y);
    virtual b2Vec2 getVelocity();
    virtual void updateCurrent(sf::Time deltaTime) = 0;

private:
    b2Vec2 mVelocity;
};