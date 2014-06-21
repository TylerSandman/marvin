#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Marvin.h"
#include "ResourceManager.h"
#include "Command.h"

Marvin::Marvin(TextureManager &textureManager, b2Body *playerBody) : 
    Entity(playerBody),
    mAttachedPlatform(nullptr),
    mControlledVelocity(getVelocity()),
    mFading(false){

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

    Animation fadeAnimation;
    fadeAnimation.setSpriteSheet(spriteSheet);
    fadeAnimation.addFrame(sf::IntRect(0, 100, 70, 100));
    fadeAnimation.addFrame(sf::IntRect(70, 100, 70, 100));
    fadeAnimation.addFrame(sf::IntRect(140, 100, 70, 100));
    fadeAnimation.addFrame(sf::IntRect(210, 100, 70, 100));
    fadeAnimation.addFrame(sf::IntRect(280, 100, 70, 100));
    fadeAnimation.addFrame(sf::IntRect(350, 100, 70, 100));
    mAnimationMap[AnimationID::Fade] = fadeAnimation;

    mSprite = AnimatedSprite(sf::seconds(0.1f));
    mSprite.setAnimation(walkingAnimation);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mCurrentFacingDirection = Entity::FacingDirection::Left;
    turn(Entity::FacingDirection::Right);    
    numFootContacts = 0;
}

unsigned int Marvin::getCategory(){
    return Category::Player;
}

void Marvin::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    if (!isFaded()){
        target.draw(mSprite, states);
        //debugDraw(target, states);
    }
}

void Marvin::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * 70.f;
    mSprite.setPosition(physPos.x * 70.f, mapHeight - physPos.y * 70.f);
    mPreviousPosition = physPos;
    if (isOnGround() && !mFading)
        mSprite.play(mAnimationMap[Marvin::AnimationID::Walk]);
    else if (!mFading)
        mSprite.play(mAnimationMap[Marvin::AnimationID::Jump]);
    mSprite.update(deltaTime);
}

void Marvin::setControlledVelocity(b2Vec2 velocity){
    mControlledVelocity = velocity;
}

b2Vec2 Marvin::getControlledVelocity(){
    return mControlledVelocity;
}

void Marvin::attachPlatform(GrassPlatform *platform){
    mAttachedPlatform = platform;
}

void Marvin::detachPlatform(){
    mAttachedPlatform = nullptr;
}

GrassPlatform* Marvin::getAttachedPlatform() const{
    return mAttachedPlatform;
}

bool Marvin::isOnGround() const{
    return (numFootContacts > 0);
}

bool Marvin::isFaded() const{
    return (mFading && !mSprite.isPlaying());
}

void Marvin::setNumFootContacts(int num){
    numFootContacts = num;
}

int Marvin::getNumFootContacts(){
    return numFootContacts;
}

void Marvin::turn(Entity::FacingDirection direction){
    if ((mCurrentFacingDirection == Entity::FacingDirection::Left) && 
        (direction == Entity::FacingDirection::Right)){
        mSprite.setScale(-1.f, 1.f);
        mCurrentFacingDirection = Entity::FacingDirection::Right;
    }
    else if ((mCurrentFacingDirection == Entity::FacingDirection::Right) && 
             (direction == Entity::FacingDirection::Left)){
        mSprite.setScale(1.f, 1.f);
        mCurrentFacingDirection = Entity::FacingDirection::Left;
    }
    mSprite.play();
}

void Marvin::stopAnimation(){
    //Need this check because during world restarts
    //we may try to stop non-existant animations due
    //to player resetting
    if ((mSprite.getAnimation()->getSize() > 0) &&
        (mSprite.getAnimation()->getSize() < 10) &&
        (mSprite.isLooped()))
        mSprite.stop();
}

void Marvin::fade(){
    mFading = true;
    mSprite.play(mAnimationMap[AnimationID::Fade]);
    mSprite.setLooped(false);
}

void Marvin::debugDraw(sf::RenderTarget &target, sf::RenderStates states) const{

    b2Vec2 position = mB2Body->GetPosition();
    b2PolygonShape *footShape = static_cast<b2PolygonShape*>(mB2Body->GetFixtureList()->GetShape());
    b2Vec2 v1 = footShape->GetVertex(0);
    b2Vec2 v2 = footShape->GetVertex(1);
    b2Vec2 v3 = footShape->GetVertex(2);
    b2Vec2 v4 = footShape->GetVertex(3);

    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * 70.f;
    sf::Vertex renderV1(sf::Vector2f(position.x * 70.f + v1.x * 70.f, mapHeight - (position.y * 70.f + v1.y * 70.f)), sf::Color::Blue); 
    sf::Vertex renderV2(sf::Vector2f(position.x * 70.f + v2.x * 70.f, mapHeight - (position.y * 70.f + v2.y * 70.f)), sf::Color::Blue); 
    sf::Vertex renderV3(sf::Vector2f(position.x * 70.f + v3.x * 70.f, mapHeight - (position.y * 70.f + v3.y * 70.f)), sf::Color::Blue); 
    sf::Vertex renderV4(sf::Vector2f(position.x * 70.f + v4.x * 70.f, mapHeight - (position.y * 70.f + v4.y * 70.f)), sf::Color::Blue); 
    sf::VertexArray lines(sf::PrimitiveType::Lines);
    lines.append(renderV1);
    lines.append(renderV2);
    lines.append(renderV3);
    lines.append(renderV4);
    target.draw(lines, states);
}