#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include "ResourceManager.h"
#include "Button.h"

namespace GUI{

Button::Button(TextureManager &textureManager, FontManager &fontManager, Component *parent) :
        Component(parent),
        mTexture(textureManager.get(TextureID::Button)),
        mText("", fontManager.get(FontID::Main), 24),
        mIsToggle(false){

    mSprite.setTexture(mTexture);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}

void Button::setOnClick(Callback callback){
    mOnClick = callback;
}

void Button::setText(const std::string &text, sf::Color color){
    mText.setString(text);
    mText.setColor(color);
    sf::FloatRect textBounds = mText.getGlobalBounds();
    mText.setOrigin(textBounds.width/2, textBounds.height/2);
    mText.setPosition(0.f, -textBounds.height/2);
    mDefaultColor = color;
}

void Button::setToggle(bool toggle){
    mIsToggle = toggle;
}

bool Button::isSelectable(){
    return false;
}

void Button::handleEvent(const sf::Event &event){

    sf::FloatRect buttonRect = getRect();
    //Hover behaviour
    if (event.type == sf::Event::MouseMoved){
        
        float mouseX = static_cast<float>(event.mouseMove.x);
        float mouseY = static_cast<float>(event.mouseMove.y);

        if (buttonRect.contains(mouseX, mouseY))
            mText.setColor(sf::Color::Black);
        else
            mText.setColor(mDefaultColor);
    }

    //Handle button clicks
    if ((event.type == sf::Event::MouseButtonPressed) &&
        (event.mouseButton.button == sf::Mouse::Button::Left)) {

        float mouseX = static_cast<float>(event.mouseButton.x);
        float mouseY = static_cast<float>(event.mouseButton.y);
                
        if ((buttonRect.contains(mouseX, mouseY)) && (!isSelected()))
            if (mOnClick) mOnClick();
    }
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}

sf::FloatRect Button::getRect() const{
    sf::Vector2f origin = mSprite.getOrigin();
    sf::Vector2f pos = getAbsolutePosition();
    sf::FloatRect bounds = mSprite.getLocalBounds();
    sf::FloatRect buttonRect(
        pos.x - origin.x,
        pos.y - origin.y,
        bounds.width,
        bounds.height);
    return buttonRect;
}
}