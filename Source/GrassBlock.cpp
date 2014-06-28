#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <cmath>
#include "GrassBlock.h"
#include "ResourceManager.h"
#include "Command.h"
#include "Constants.h"

GrassBlock::GrassBlock(TextureManager &textureManager, b2Body *enemyBody, float jumpHeight) : 
    Entity(enemyBody),
    mBouncing(false),
    jumpVelocity(sqrt(fabs(2 * phys::GRAVITY * jumpHeight))){

    sf::Texture &spriteSheet = textureManager.get(TextureID::EnemiesSpriteSheet);
    
    Animation bounceAnimation;
    bounceAnimation.setSpriteSheet(spriteSheet);
    bounceAnimation.addFrame(sf::IntRect(0, 141, 71, 70));
    bounceAnimation.addFrame(sf::IntRect(0, 211, 71, 70));
    mAnimation = bounceAnimation;

    mSprite = AnimatedSprite(sf::seconds(0.035f));
    mSprite.setAnimation(mAnimation);
    stopAnimation();
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}

unsigned int GrassBlock::getCategory() const{
    return Category::GrassBlock;
}

void GrassBlock::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void GrassBlock::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;
    mSprite.setPosition(physPos.x * PX_PER_M, mapHeight - physPos.y * PX_PER_M);
    mPreviousPosition = physPos;
    mSprite.update(deltaTime);
    if (getVelocity().y == 0 && !mBouncing){
        mSprite.play();
        mSprite.setLooped(false);
        mBouncing = true;
    }
    if (getVelocity().y == 0 && !mSprite.isPlaying()){
        jump();
        mBouncing = false;
    }
}

void GrassBlock::jump(){
    setVelocity(b2Vec2(getVelocity().x, jumpVelocity));
}

void GrassBlock::stopAnimation(){
    //Need this check because during world restarts
    //we may try to stop non-existant animations due
    //to player resetting
    if ((mSprite.getAnimation()->getSize() > 0) &&
        (mSprite.getAnimation()->getSize() < 10))
        mSprite.stop();
}