#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Barnacle.h"
#include "ResourceManager.h"
#include "Command.h"
#include "Constants.h"

Barnacle::Barnacle(TextureManager &textureManager, b2Body *enemyBody, bool flipped) : 
    Entity(enemyBody){

    sf::Texture &spriteSheet = textureManager.get(TextureID::EnemiesSpriteSheet);
    
    Animation biteAnimation;
    biteAnimation.setSpriteSheet(spriteSheet);
    biteAnimation.addFrame(sf::IntRect(318, 239, 51, 58));
    biteAnimation.addFrame(sf::IntRect(528, 220, 51, 58));
    mAnimation = biteAnimation;

    mSprite = AnimatedSprite(sf::seconds(0.15f));
    mSprite.setAnimation(mAnimation);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);

    if (flipped){
        mSprite.setScale(1.f, -1.f);
    }
}

unsigned int Barnacle::getCategory() const{
    return Category::Enemy;
}

void Barnacle::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void Barnacle::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;
    mSprite.setPosition(physPos.x * PX_PER_M, mapHeight - physPos.y * PX_PER_M);
    mPreviousPosition = physPos;
    mSprite.update(deltaTime);
}

void Barnacle::stopAnimation(){
    //Need this check because during world restarts
    //we may try to stop non-existant animations due
    //to player resetting
    if ((mSprite.getAnimation()->getSize() > 0) &&
        (mSprite.getAnimation()->getSize() < 10))
        mSprite.stop();
}