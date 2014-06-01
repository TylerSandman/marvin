#pragma once
#include <Box2D\Box2D.h>
#include "AnimatedSprite.h"
#include "SceneNode.h"
#include "ResourceManager.h"

class Entity : public SceneNode{

public:
    Entity(b2Body *entityBody);
    typedef std::unique_ptr<Entity> Ptr;
    sf::Vector2f getRenderPosition();
    void setRenderPosition(sf::Vector2f position);
    b2Vec2 getPosition();
    void setVelocity(b2Vec2 velocity);
    b2Vec2 getVelocity(); 
    sf::FloatRect getBoundingBox();

protected:
    b2Vec2 mVelocity;
    b2Vec2 mPreviousPosition;
    AnimatedSprite mSprite;
    b2Body *mB2Body;
};