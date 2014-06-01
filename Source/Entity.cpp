#include <Box2D\Box2D.h>
#include "Entity.h"

Entity::Entity(b2Body *entityBody) : 
    mB2Body(entityBody), mPreviousPosition(entityBody->GetPosition()){}
sf::Vector2f Entity::getRenderPosition(){
    return mSprite.getPosition();
}

void Entity::setRenderPosition(sf::Vector2f position){
    mSprite.setPosition(position);
}

b2Vec2 Entity::getPosition(){
    return mB2Body->GetPosition();
}

void Entity::setVelocity(b2Vec2 velocity){
    mB2Body->SetLinearVelocity(velocity);
}

b2Vec2 Entity::getVelocity(){
    return mB2Body->GetLinearVelocity();
}

sf::FloatRect Entity::getBoundingBox(){
    return mSprite.getGlobalBounds();
}