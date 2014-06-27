#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include "ResourceManager.h"
#include "Button.h"

namespace GUI{

Button::Button(TextureManager &textureManager) : 
    mTextureNormalRect(sf::IntRect(1, 1, 228, 49)),
    mTexturePressedRect(sf::IntRect(1, 51, 228, 49)),
    mTextureDisabledRect(sf::IntRect(1, 101, 228, 49)),
    mIsToggle(false),
    mEnabled(true){}

Button::Button(TextureManager &textureManager, FontManager &fontManager) :
        mTextureNormalRect(sf::IntRect(1, 1, 228, 49)),
        mTexturePressedRect(sf::IntRect(1, 51, 228, 49)),
        mTextureDisabledRect(sf::IntRect(1, 101, 228, 49)),
        mText("", fontManager.get(FontID::Main), 24),
        mIsToggle(false),
        mEnabled(true){

    mSprite.setTexture(textureManager.get(TextureID::GUISpriteSheet));
    mSprite.setTextureRect(mTextureNormalRect);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}

void Button::setText(const std::string &text, sf::Color color){
    mText.setString(text);
    mText.setColor(color);
    sf::FloatRect textBounds = mText.getGlobalBounds();
    mText.setOrigin(textBounds.width/2, textBounds.height/2);
    mText.setPosition(0.f, -textBounds.height/2);
}

void Button::setCallback(Callback callback){
    mCallback = callback;
}

void Button::setToggle(bool toggle){
    mIsToggle = toggle;
}

bool Button::isSelectable(){
    return mEnabled;
}

void Button::enable(){
    mEnabled = true;
    mText.setColor(sf::Color::Black);
    changeTexture(Type::Normal);
}

void Button::disable(){
    mEnabled = false;
    mText.setColor(sf::Color(0,0,0,100));
    changeTexture(Type::Disabled);
}

void Button::select(){

    Component::select();
    changeTexture(Type::Pressed);
}

void Button::deselect(){
    Component::deselect();
    changeTexture(Type::Normal);
}

void Button::activate(){

    Component::activate();

    if (mIsToggle)
        changeTexture(Type::Pressed);

    if (mCallback)
        mCallback();

    if (!mIsToggle)
        deactivate();
    
}

void Button::deactivate(){

    Component::deactivate();

    if (mIsToggle){
        if (isSelected())
            changeTexture(Type::Pressed);
        else
            changeTexture(Type::Normal);
    }
}


void Button::handleEvent(const sf::Event &event){}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
}

void Button::changeTexture(Type buttonType){

    switch(buttonType){
    case(Type::Normal):
        mSprite.setTextureRect(mTextureNormalRect);
        break;
    case(Type::Pressed):
        mSprite.setTextureRect(mTexturePressedRect);
        break;
    case(Type::Disabled):
        mSprite.setTextureRect(mTextureDisabledRect);
        break;

    }
}
}