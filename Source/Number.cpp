#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <boost/math/special_functions/round.hpp>
#include <iostream>
#include <cmath>
#include "ResourceManager.h"
#include "Number.h"

namespace GUI{

Number::Number(TextureManager &textureManager, unsigned int num) :
        mOnesSprite(),
        mTensSprite(),
        mTenthsSprite(),
        mHundredthsSprite(),
        mDecimalSprite(){

    int numTens = num / 10;
    int numOnes = num % 10;

    mOnesSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    if (numTens > 0)
        mTensSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    mOnesSprite.setTextureRect(getRectFromNum(numOnes));
    if (numTens > 0)
        mTensSprite.setTextureRect(getRectFromNum(numTens));

    sf::FloatRect onesBounds = mOnesSprite.getGlobalBounds();
    sf::FloatRect tensBounds = mTensSprite.getGlobalBounds();
    mOnesSprite.setOrigin(onesBounds.width / 2.f, onesBounds.height / 2.f);
    mTensSprite.setOrigin(tensBounds.width / 2.f, tensBounds.height / 2.f);

    if (mTensSprite.getTexture()){
        mTensSprite.move(-16.f, 0.f);
        mOnesSprite.move(16.f, 0.f);
    }
}

Number::Number(TextureManager &textureManager, float num) :
        mOnesSprite(),
        mTensSprite(),
        mTenthsSprite(),
        mHundredthsSprite(),
        mDecimalSprite(){

    int numInt = static_cast<int>(std::floor(num));
    int numTens = numInt / 10;
    int numOnes = numInt % 10;

    float decRemainder = boost::math::round(100.f * fmod(num, std::floor(num))) / 100.f;
    int decimal = static_cast<int>(100 * decRemainder);
    int numTenths = decimal / 10;
    int numHundredths = decimal % 10;

    mOnesSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    if (numTens > 0)
        mTensSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    mOnesSprite.setTextureRect(getRectFromNum(numOnes));
    if (numTens > 0)
        mTensSprite.setTextureRect(getRectFromNum(numTens));
    mHundredthsSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    mHundredthsSprite.setTextureRect(getRectFromNum(numHundredths));
    mTenthsSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    mTenthsSprite.setTextureRect(getRectFromNum(numTenths));
    mDecimalSprite.setTexture(textureManager.get(TextureID::HUDSpriteSheet));
    mDecimalSprite.setTextureRect(sf::IntRect(168, 0, 8, 38));

    sf::FloatRect onesBounds = mOnesSprite.getGlobalBounds();
    sf::FloatRect tensBounds = mTensSprite.getGlobalBounds();
    sf::FloatRect hundredthsBounds = mHundredthsSprite.getGlobalBounds();
    sf::FloatRect tenthsBounds = mTenthsSprite.getGlobalBounds();
    sf::FloatRect decimalBounds = mDecimalSprite.getGlobalBounds();
    mOnesSprite.setOrigin(onesBounds.width / 2.f, onesBounds.height / 2.f);
    mTensSprite.setOrigin(tensBounds.width / 2.f, tensBounds.height / 2.f);
    mHundredthsSprite.setOrigin(hundredthsBounds.width / 2.f, hundredthsBounds.height / 2.f);
    mTenthsSprite.setOrigin(tenthsBounds.width / 2.f, tenthsBounds.height / 2.f);
    mDecimalSprite.setOrigin(decimalBounds.width / 2.f, decimalBounds.height / 2.f);

    if (mTensSprite.getTexture()){
        mTensSprite.move(-52.f, 0.f);     
    }
    mOnesSprite.move(-20.f, 0.f);
    mTenthsSprite.move(20.f, 0.f);
    mHundredthsSprite.move(52.f, 0.f);
}

bool Number::isSelectable(){
    return false;
}

void Number::handleEvent(const sf::Event &event){}

void Number::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mTensSprite, states);
    target.draw(mOnesSprite, states);
    target.draw(mTenthsSprite, states);
    target.draw(mHundredthsSprite, states);
    target.draw(mDecimalSprite, states);
}

sf::IntRect Number::getRectFromNum(unsigned int num){

    switch(num){
    case 0:
        return sf::IntRect(230, 0, 30, 38);
    case 1:
        return sf::IntRect(196, 41, 26, 37);
    case 2:
        return sf::IntRect(55, 98, 32, 38);
    case 3:
        return sf::IntRect(239, 80, 28, 38);
    case 4:
        return sf::IntRect(238, 122, 29, 38);
    case 5:
        return sf::IntRect(238, 162, 28, 38);
    case 6:
        return sf::IntRect(230, 40, 30, 38);
    case 7:
        return sf::IntRect(226, 206, 32, 39);
    case 8:
        return sf::IntRect(192, 206, 32, 40);
    case 9:
        return sf::IntRect(196, 0, 32, 39);
    default:
        return sf::IntRect(230, 0, 30, 38);
    }
}
}