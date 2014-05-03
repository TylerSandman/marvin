#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Entity.h"
#include "ResourceManager.h"

class Marvin : public Entity{
    
public:
    Marvin(TextureManager &textureManager, b2Body *playerBody);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states);

private:
    b2Body *mB2Body;
    sf::Sprite mSprite;
};