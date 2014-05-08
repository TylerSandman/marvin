#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Marvin.h"
#include "ResourceManager.h"
#include "Command.h"

Marvin::Marvin(TextureManager &textureManager, b2Body *playerBody) : 
    mB2Body(playerBody), mPreviousPosition(playerBody->GetPosition()){

    mSprite.setTexture(textureManager.get(TextureID::PlayerStanding));
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mState = State::OnGround;
}

unsigned int Marvin::getCategory(){
    return Category::Player;
}

void Marvin::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void Marvin::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * 70.f;
    mSprite.setPosition(physPos.x * 70.f, mapHeight - physPos.y * 70.f);
    mPreviousPosition = physPos;

    //TODO MANAGE ANIMATIONS/SPRITES BASED ON STATE
}

sf::Vector2f Marvin::getRenderPosition(){
    return mSprite.getPosition();
}

void Marvin::setRenderPosition(sf::Vector2f position){
    mSprite.setPosition(position);
}

b2Vec2 Marvin::getPosition(){
    return mB2Body->GetPosition();
}

void Marvin::setVelocity(b2Vec2 velocity){
    mB2Body->SetLinearVelocity(velocity);
}

b2Vec2 Marvin::getVelocity(){
    return mB2Body->GetLinearVelocity();
}

sf::FloatRect Marvin::getBoundingBox(){
    return mSprite.getGlobalBounds();
}

Marvin::State Marvin::getState(){
    return mState;
}

void Marvin::setState(Marvin::State state){
    mState = state;
}