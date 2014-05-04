#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Entity.h"
#include "ResourceManager.h"

class Marvin : public Entity{
    
public:
    Marvin(TextureManager &textureManager, b2Body *playerBody);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    sf::Vector2f getRenderPosition();
    void setRenderPosition(sf::Vector2f position);
    b2Vec2 getPosition();   
    sf::FloatRect getBoundingBox();

private:
    b2Body *mB2Body;
    sf::Sprite mSprite;
    b2Vec2 mPreviousPosition;
};