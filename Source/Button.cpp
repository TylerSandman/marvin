#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include "ResourceManager.h"
#include "Button.h"

namespace GUI{

Button::Button(TextureManager &textureManager, Component *parent) :
        Component(parent),
        mTexture(textureManager.get(TextureID::Button)),
        mIsToggle(false){

    mSprite.setTexture(mTexture);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    mText.setPosition(bounds.width/2, bounds.height/2);
}

void Button::setCallback(Callback callback){
    mCallback = callback;
}

void Button::setText(const std::string &text){
    mText.setString(text);
    sf::FloatRect bounds = mText.getGlobalBounds();
    mText.setOrigin(bounds.width/2, bounds.height/2);
}

void Button::setToggle(bool toggle){
    mIsToggle = toggle;
}

bool Button::isSelectable(){
    return true;
}

void Button::select(){

    Component::select();

    if (mCallback)
        mCallback();

    if (!mIsToggle)
        deselect();
}

void Button::deselect(){

    Component::deselect();
}

void Button::handleEvent(const sf::Event &event){

    //Handle button clicks
    if ((event.type == sf::Event::MouseButtonPressed) &&
        (event.mouseButton.button == sf::Mouse::Button::Left)) {

        float mouseX = event.mouseButton.x;
        float mouseY = event.mouseButton.y;
        sf::Vector2f origin = mSprite.getOrigin();
        sf::Vector2f pos = getAbsolutePosition();
        sf::FloatRect bounds = mSprite.getLocalBounds();
        sf::FloatRect buttonRect(
            pos.x - origin.x,
            pos.y - origin.y,
            bounds.width,
            bounds.height);
        
        if ((buttonRect.contains(mouseX, mouseY)) && (!isSelected())){
            select();
        }

    }
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
}
}