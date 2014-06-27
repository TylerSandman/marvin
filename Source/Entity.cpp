#include <Box2D\Box2D.h>
#include "Entity.h"

Entity::Entity(b2Body *entityBody) : 
    mB2Body(entityBody), mPreviousPosition(entityBody->GetPosition()){}

sf::Vector2f Entity::getRenderPosition() const{
    return mSprite.getPosition();
}

void Entity::setRenderPosition(sf::Vector2f position){
    mSprite.setPosition(position);
}

b2Vec2 Entity::getPosition() const{
    return mB2Body->GetPosition();
}

void Entity::setVelocity(b2Vec2 velocity){
    mB2Body->SetLinearVelocity(velocity);
}

b2Vec2 Entity::getVelocity() const{
    return mB2Body->GetLinearVelocity();
}

sf::FloatRect Entity::getBoundingBox() const{
    return mSprite.getGlobalBounds();
}