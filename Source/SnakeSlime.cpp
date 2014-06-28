#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "SnakeSlime.h"
#include "ResourceManager.h"
#include "Command.h"
#include "Constants.h"

SnakeSlime::SnakeSlime(TextureManager &textureManager, b2Body *enemyBody) : 
    Entity(enemyBody){

    sf::Texture &spriteSheet = textureManager.get(TextureID::EnemiesSpriteSheet);
    
    Animation wiggleAnimation;
    wiggleAnimation.setSpriteSheet(spriteSheet);
    wiggleAnimation.addFrame(sf::IntRect(424, 187, 53, 147));
    wiggleAnimation.addFrame(sf::IntRect(425, 40, 52, 147));
    mAnimation = wiggleAnimation;

    mSprite = AnimatedSprite(sf::seconds(0.15f));
    mSprite.setAnimation(mAnimation);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);
}

unsigned int SnakeSlime::getCategory() const{
    return Category::Enemy;
}

void SnakeSlime::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void SnakeSlime::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * PX_PER_M;
    mSprite.setPosition(physPos.x * PX_PER_M, mapHeight - physPos.y * PX_PER_M);
    mPreviousPosition = physPos;
    mSprite.update(deltaTime);
}

void SnakeSlime::stopAnimation(){
    //Need this check because during world restarts
    //we may try to stop non-existant animations due
    //to player resetting
    if ((mSprite.getAnimation()->getSize() > 0) &&
        (mSprite.getAnimation()->getSize() < 10))
        mSprite.stop();
}