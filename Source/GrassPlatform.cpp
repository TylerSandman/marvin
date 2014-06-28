#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <cmath>
#include "GrassPlatform.h"
#include "ResourceManager.h"
#include "Command.h"
#include "Constants.h"

GrassPlatform::GrassPlatform(TextureManager &textureManager, b2Body *enemyBody, float moveVelocity, std::vector<sf::Vector2f> waypoints, bool large) : 
    Entity(enemyBody),
    mMoving(false),
    mMoveVelocity(moveVelocity){

    sf::Texture &spriteSheet = textureManager.get(TextureID::EnemiesSpriteSheet);
    Animation animation;
    animation.setSpriteSheet(spriteSheet);
    if (large)
        animation.addFrame(sf::IntRect(210, 0, 585, 39));
    else
        animation.addFrame(sf::IntRect(579, 272, 209, 39));
    mAnimation = animation;
    mSprite = AnimatedSprite();
    mSprite.setAnimation(mAnimation);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);

    //Position waypoints according to the platform
    //Rather than absolute top-left coordinates
    for (auto &waypoint : waypoints){
        sf::Vector2f platformWaypoint(
            waypoint.x + 35.f,
            waypoint.y + PX_PER_M - bounds.height / 2.f);
        mWaypoints.push_back(platformWaypoint);
    }
    mCurrentWaypoint = mWaypoints.begin();
}

unsigned int GrassPlatform::getCategory() const{
    return Category::GrassPlatform;
}

void GrassPlatform::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void GrassPlatform::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;
    mSprite.setPosition(physPos.x * PX_PER_M, mapHeight - physPos.y * PX_PER_M);
    mPreviousPosition = physPos;
    b2Vec2 currentWaypointPhysPos(
        (*mCurrentWaypoint).x / PX_PER_M,
        mapHeight / PX_PER_M - (*mCurrentWaypoint).y / PX_PER_M);

    //If the platform is close enough to its next waypoint advance to the next one
    sf::Vector2f distanceVector = mSprite.getPosition() - *mCurrentWaypoint;
    float mag = sqrt(pow(distanceVector.x, 2) + pow(distanceVector.y, 2));
    if ((mag < 10.f) && (mMoving)){
        mCurrentWaypoint++;
        if (mCurrentWaypoint == mWaypoints.end()){
            std::reverse(mWaypoints.begin(), mWaypoints.end());
            mCurrentWaypoint = mWaypoints.begin();
            deactivate();
        }
    }
    
    
    else if (mMoving){
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

void GrassPlatform::activate(){
    mMoving = true;
}

void GrassPlatform::deactivate(){
    setVelocity(b2Vec2_zero);
    mMoving = false;
}