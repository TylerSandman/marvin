#include <Box2D\Box2D.h>
#include "Entity.h"

void Entity::setVelocity(b2Vec2 velocity){
    mVelocity = velocity;
}

void Entity::setVelocity(float x, float y){
    mVelocity = b2Vec2(x, y);
}

b2Vec2 Entity::getVelocity(){ 
    return mVelocity; 
}