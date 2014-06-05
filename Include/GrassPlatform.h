#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include <vector>
#include "Entity.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"

class GrassPlatform : public Entity{
    
public:
    GrassPlatform(TextureManager &textureManager, b2Body *enemyBody, float moveVelocity, std::vector<sf::Vector2f> waypoints);
    unsigned int getCategory();
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void activate();
    void deactivate();

private:
    std::vector<sf::Vector2f> mWaypoints;
    std::vector<sf::Vector2f>::iterator mCurrentWaypoint;
    Animation mAnimation;
    bool mMoving;
    float mMoveVelocity;

};