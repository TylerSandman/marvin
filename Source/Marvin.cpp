#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Marvin.h"
#include "ResourceManager.h"
#include "Command.h"

Marvin::Marvin(TextureManager &textureManager, b2Body *playerBody) : 
    mB2Body(playerBody), mPreviousPosition(playerBody->GetPosition()){

    sf::Texture &spriteSheet = textureManager.get(TextureID::PlayerSpriteSheet);
    
    Animation walkingAnimation;
    walkingAnimation.setSpriteSheet(spriteSheet);
    walkingAnimation.addFrame(sf::IntRect(0, 0, 70, 100));
    walkingAnimation.addFrame(sf::IntRect(70, 0, 70, 100));
    walkingAnimation.addFrame(sf::IntRect(140, 0, 70, 100));
    mAnimationMap[AnimationID::Walk] = walkingAnimation;

    Animation jumpingAnimation;
    jumpingAnimation.setSpriteSheet(spriteSheet);
    jumpingAnimation.addFrame(sf::IntRect(210, 0, 70, 100));
    mAnimationMap[AnimationID::Jump] = jumpingAnimation;
    
    Animation hurtAnimation;
    hurtAnimation.setSpriteSheet(spriteSheet);
    hurtAnimation.addFrame(sf::IntRect(280, 0, 70, 100));
    mAnimationMap[AnimationID::Hurt] = hurtAnimation;

    mSprite = AnimatedSprite(sf::seconds(0.1f));
    mSprite.setAnimation(walkingAnimation);
    mCurrentAnimationID = AnimationID::Walk;
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mCurrentFacingDirection = Marvin::FacingDirection::Left;
    turn();    
    numFootContacts = 0;
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
    if (isOnGround())
        mSprite.play(mAnimationMap[Marvin::AnimationID::Walk]);
    else
        mSprite.play(mAnimationMap[Marvin::AnimationID::Jump]);
    if (mCurrentAnimationID == Marvin::AnimationID::None)
        mSprite.stop();
    mSprite.update(deltaTime);
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

bool Marvin::isOnGround(){
    return (numFootContacts > 0);
}

Marvin::AnimationID Marvin::getAnimationID(){
    return mCurrentAnimationID;
}

void Marvin::setAnimationID(Marvin::AnimationID id){
    mCurrentAnimationID = id;
}

void Marvin::setFacingDirection(Marvin::FacingDirection direction){
    mCurrentFacingDirection = direction;
}

Marvin::FacingDirection Marvin::getFacingDirection(){
    return mCurrentFacingDirection;
}

void Marvin::turn(){
    if (mCurrentFacingDirection == Marvin::FacingDirection::Left){
        mSprite.setScale(-1.f, 1.f);
        mCurrentFacingDirection = Marvin::FacingDirection::Right;
    }
    else{
        mSprite.setScale(1.f, 1.f);
        mCurrentFacingDirection = Marvin::FacingDirection::Left;
    }
}

void Marvin::setNumFootContacts(int num){
    numFootContacts = num;
}

int Marvin::getNumFootContacts(){
    return numFootContacts;
}