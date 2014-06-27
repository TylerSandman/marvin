#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include "Entity.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"

class GrassBlock : public Entity{
    
public:
    GrassBlock(TextureManager &textureManager, b2Body *enemyBody, float jumpVelocity);
    unsigned int getCategory() const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void jump();
    void stopAnimation(); 

private:
    Animation mAnimation;
    bool mBouncing;
    float jumpVelocity;
};