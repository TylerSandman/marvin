#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Marvin.h"
#include "ResourceManager.h"
#include "Command.h"

Marvin::Marvin(TextureManager &textureManager, b2Body *playerBody) : 
    Entity(playerBody){

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
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mCurrentFacingDirection = Marvin::FacingDirection::Left;
    turn(Marvin::FacingDirection::Right);    
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
    mSprite.update(deltaTime);
}

bool Marvin::isOnGround(){
    return (numFootContacts > 0);
}

void Marvin::setNumFootContacts(int num){
    numFootContacts = num;
}

int Marvin::getNumFootContacts(){
    return numFootContacts;
}

void Marvin::turn(Marvin::FacingDirection direction){
    if ((mCurrentFacingDirection == Marvin::FacingDirection::Left) && 
        (direction == Marvin::FacingDirection::Right)){
        mSprite.setScale(-1.f, 1.f);
        mCurrentFacingDirection = Marvin::FacingDirection::Right;
    }
    else if ((mCurrentFacingDirection == Marvin::FacingDirection::Right) && 
             (direction == Marvin::FacingDirection::Left)){
        mSprite.setScale(1.f, 1.f);
        mCurrentFacingDirection = Marvin::FacingDirection::Left;
    }
    mSprite.play();
}

void Marvin::stopAnimation(){
    //Need this check because during world restarts
    //we may try to stop non-existant animations due
    //to player resetting
    if ((mSprite.getAnimation()->getSize() > 0) &&
        (mSprite.getAnimation()->getSize() < 10))
        mSprite.stop();
}