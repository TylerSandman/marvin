#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Marvin.h"
#include "ResourceManager.h"

Marvin::Marvin(TextureManager &textureManager, b2Body *playerBody){
    mSprite.setTexture(textureManager.get(TextureID::PlayerStanding));
    mB2Body = playerBody;
}

void Marvin::drawCurrent(sf::RenderTarget &target, sf::RenderStates states){
    target.draw(mSprite, states);
}