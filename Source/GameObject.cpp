#include <SFML\Graphics.hpp>
#include "Command.h"
#include "GameObject.h"

GameObject::GameObject(Category::Type category, sf::Sprite &sprite, b2Body *objectBody) :
    mCategory(category), mSprite(sprite), mBody(objectBody){

    //Center our origin so we play nice with Box2D
    mSprite.setOrigin(
        mSprite.getGlobalBounds().width/2,
        mSprite.getGlobalBounds().height/2);
    mSprite.move(
        mSprite.getGlobalBounds().width/2,
        mSprite.getGlobalBounds().height/2);
}

unsigned int GameObject::getCategory(){
    return mCategory;
}

void GameObject::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}