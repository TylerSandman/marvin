#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include <vector>
#include "Entity.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"

class Bee : public Entity{
    
public:
    Bee(TextureManager &textureManager, b2Body *enemyBody, float moveVelocity, std::vector<sf::Vector2f> waypoints, float steering = 3.f);
    unsigned int getCategory() const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void seek(sf::Vector2f pos);

private:
    void followWaypoints();

private:
    std::vector<sf::Vector2f> mWaypoints;
    std::vector<sf::Vector2f>::iterator mCurrentWaypoint;
    Animation mAnimation;
    float mMoveVelocity;
    float mSteering;
    bool mSeeking;
    Entity::FacingDirection mCurrentFacingDirection;
};