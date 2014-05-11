#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>
#include "ResourceManager.h"
#include "ContainerButton.h"

namespace GUI{

ContainerButton::ContainerButton(TextureManager &textureManager) :
        Button(textureManager),
        mChildren(){

    mTextureNormal = textureManager.get(TextureID::GreenContainerButton);
    mTexturePressed = textureManager.get(TextureID::GreenContainerButtonPressed);
    mTextureDisabled = textureManager.get(TextureID::ContainerButtonDisabled);
    mSprite.setTexture(mTextureNormal);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}

void ContainerButton::enable(){
    mEnabled = true;
    changeTexture(Type::Normal);
}

void ContainerButton::disable(){
    mEnabled = false;
    changeTexture(Type::Disabled);
}

void ContainerButton::add(Component::Ptr component){
    mChildren.push_back(component);
}

void ContainerButton::handleEvent(const sf::Event &event){}

void ContainerButton::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mSprite, states);
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