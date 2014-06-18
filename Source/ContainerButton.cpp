#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include "ResourceManager.h"
#include "ContainerButton.h"

namespace GUI{

ContainerButton::ContainerButton(TextureManager &textureManager) :
        Button(textureManager),
        mChildren(),
        mPointer(textureManager.get(TextureID::GreenContainerButtonPointer)){

    mTextureNormal = textureManager.get(TextureID::GreenContainerButton);
    mTexturePressed = textureManager.get(TextureID::GreenContainerButtonPressed);
    mTextureDisabled = textureManager.get(TextureID::ContainerButtonDisabled);
    mSprite.setTexture(mTextureNormal);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
    sf::FloatRect pointerBounds = mPointer.getLocalBounds();
    mPointer.setOrigin(pointerBounds.width / 2.f, pointerBounds.height / 2.f);
    mPointer.move(-bounds.width / 2.f - pointerBounds.width / 2.f - 10.f, 0.f);
}

bool ContainerButton::isSelectable(){
    return true;
}

void ContainerButton::activate(){

    if (mEnabled){
        Component::activate();

        if (mIsToggle)
            changeTexture(Type::Pressed);

        if (mCallback)
            mCallback();

        if (!mIsToggle)
            deactivate();
    }  
}

void ContainerButton::deactivate(){

    if (mEnabled){
        Component::deactivate();

        if (mIsToggle){
            if (isSelected())
                changeTexture(Type::Pressed);
            else
                changeTexture(Type::Normal);
        }
    }
}

void ContainerButton::enable(){
    mEnabled = true;
    changeTexture(Type::Normal);
}

void ContainerButton::disable(){
    mEnabled = false;
    changeTexture(Type::Disabled);
}

void ContainerButton::select(){

    Component::select();
    if (mEnabled)
        changeTexture(Type::Pressed);
}

void ContainerButton::deselect(){
    Component::deselect();
    if (mEnabled)
        changeTexture(Type::Normal);
}

void ContainerButton::add(Component::Ptr component){
    mChildren.push_back(component);
}

void ContainerButton::handleEvent(const sf::Event &event){}

void ContainerButton::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
    if (isSelected())
        target.draw(mPointer, states);
    for(auto &component : mChildren){
        target.draw(*component, states);
    }
}

void ContainerButton::changeTexture(Type buttonType){

    switch(buttonType){
    case(Type::Normal):
        mSprite.setTexture(mTextureNormal);
        break;
    case(Type::Pressed):
        mSprite.setTexture(mTexturePressed);
        break;
    case(Type::Disabled):
        mSprite.setTexture(mTextureDisabled);
        break;

    }
}
}