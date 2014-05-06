#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include "Panel.h"

namespace GUI{

Panel::Panel(TextureManager &textureManager, Component *parent) : 
        Container(parent),
        mTexture(textureManager.get(TextureID::Panel)),
        mSprite(mTexture){

    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}


void Panel::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
    for(auto &component : mChildren){
        target.draw(*component, states);
    }
}
}