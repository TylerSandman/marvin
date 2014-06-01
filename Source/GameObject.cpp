#include <SFML\Graphics.hpp>
#include "Command.h"
#include "GameObject.h"

GameObject::GameObject(Category::Type category, sf::Sprite &sprite, b2Body *objectBody) :
    mCategory(category), mSprite(sprite), mBody(objectBody){}

unsigned int GameObject::getCategory(){
    return mCategory;
}

void GameObject::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}