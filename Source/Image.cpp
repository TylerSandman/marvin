#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include "ResourceManager.h"
#include "Image.h"

namespace GUI{

Image::Image(sf::Texture &texture, Component *parent) :
        Component(parent),
        mTexture(texture){

    mSprite.setTexture(mTexture);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}

bool Image::isSelectable(){
    return false;
}

void Image::handleEvent(const sf::Event &event){}

void Image::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
}
}