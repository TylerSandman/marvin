#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include "Entity.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"

class Barnacle : public Entity{
    
public:
    Barnacle(TextureManager &textureManager, b2Body *enemyBody, bool flipped=false);
    unsigned int getCategory() const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void stopAnimation();

private:
    Animation mAnimation;
};