#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <cmath>
#include "Bee.h"
#include "ResourceManager.h"
#include "Command.h"
#include "Constants.h"

Bee::Bee(TextureManager &textureManager, b2Body *enemyBody, float moveVelocity, std::vector<sf::Vector2f> waypoints, float steering) : 
    Entity(enemyBody),
    mMoveVelocity(moveVelocity),
    mSteering(steering),
    mSeeking(false){

    sf::Texture &spriteSheet = textureManager.get(TextureID::EnemiesSpriteSheet);
    Animation animation;
    animation.setSpriteSheet(spriteSheet);
    animation.addFrame(sf::IntRect(315, 353, 56, 48));
    animation.addFrame(sf::IntRect(140, 23, 61, 42));
    mAnimation = animation;
    mSprite = AnimatedSprite(sf::seconds(0.15f));
    mSprite.setAnimation(mAnimation);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mCurrentFacingDirection = Entity::FacingDirection::Left;

    if (waypoints.empty())
        mSeeking = true;

    //Position waypoints according to the bee center
    //Rather than absolute top-left coordinates
    else{
        for (auto &waypoint : waypoints){
            sf::Vector2f platformWaypoint(
                waypoint.x + 35.f,
                waypoint.y + PX_PER_M - bounds.height / 2.f);
            mWaypoints.push_back(platformWaypoint);
        }
        mCurrentWaypoint = mWaypoints.begin();
    }
}

unsigned int Bee::getCategory() const{
    return Category::Seeker | Category::Enemy;
}

void Bee::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void Bee::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;
    mSprite.setPosition(physPos.x * PX_PER_M, mapHeight - physPos.y * PX_PER_M);
    mPreviousPosition = physPos;
    mSprite.update(deltaTime);

    if ((mCurrentFacingDirection == Entity::FacingDirection::Left) && 
        (getVelocity().x > 0)){
        mSprite.setScale(-1.f, 1.f);
        mCurrentFacingDirection = Entity::FacingDirection::Right;
    }
    else if ((mCurrentFacingDirection == Entity::FacingDirection::Right) && 
             (getVelocity().x < 0)){
        mSprite.setScale(1.f, 1.f);
        mCurrentFacingDirection = Entity::FacingDirection::Left;
    }

    if (!mSeeking)
        followWaypoints();
}

void Bee::seek(sf::Vector2f pos){

    if (!mSeeking)
        return;

    b2Vec2 physPos = mB2Body->GetPosition();
    b2Vec2 currentVelocity = getVelocity();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;

    b2Vec2 differenceVector(
            pos.x / PX_PER_M - physPos.x,
            mapHeight / PX_PER_M - pos.y / PX_PER_M - physPos.y);

    //Vector normalization
    float mag = sqrt(pow(differenceVector.x, 2) + pow(differenceVector.y, 2));
    b2Vec2 directionVector(
        differenceVector.x / mag,
        differenceVector.y / mag);

    b2Vec2 movementVector(
        directionVector.x * mMoveVelocity,
        directionVector.y * mMoveVelocity);

    //Steering behaviour
    b2Vec2 steeringVector(
        (movementVector.x - currentVelocity.x) / mSteering,
        (movementVector.y - currentVelocity.y) / mSteering);

    b2Vec2 seekVector(
        currentVelocity.x + steeringVector.x,
        currentVelocity.y + steeringVector.y);
        
    setVelocity(seekVector);
}

void Bee::followWaypoints(){

    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;

    //If close enough to its next waypoint advance to the next one
    sf::Vector2f distanceVector = mSprite.getPosition() - *mCurrentWaypoint;
    float mag = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
    if ((mag < 10.f)){
        mCurrentWaypoint++;
        if (mCurrentWaypoint == mWaypoints.end()){
            mCurrentWaypoint = mWaypoints.begin();
        }
    }   
    
    else{
        b2Vec2 differenceVector(
            (*mCurrentWaypoint).x / PX_PER_M - physPos.x,
            mapHeight / PX_PER_M - (*mCurrentWaypoint).y / PX_PER_M - physPos.y);

        //Vector normalization
        float mag = sqrt(pow(differenceVector.x, 2) + pow(differenceVector.y, 2));
        b2Vec2 directionVector(
            differenceVector.x / mag,
            differenceVector.y / mag);

        b2Vec2 movementVector(
            directionVector.x * mMoveVelocity,
            directionVector.y * mMoveVelocity);
        setVelocity(movementVector);
    }
}