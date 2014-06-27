#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include "BossSlime.h"
#include "ResourceManager.h"
#include "Command.h"

BossSlime::BossSlime(TextureManager &textureManager, b2Body *enemyBody, float velocity) : 
    Entity(enemyBody){

    sf::Texture &spriteSheet = textureManager.get(TextureID::Boss);
    
    Animation walkAnimation;
    walkAnimation.setSpriteSheet(spriteSheet);
    walkAnimation.addFrame(sf::IntRect(0, 0, 295, 186));
    walkAnimation.addFrame(sf::IntRect(0, 214, 295, 186));
    mAnimation = walkAnimation;

    mSprite = AnimatedSprite(sf::seconds(0.25f));
    mSprite.setAnimation(mAnimation);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width/2, bounds.height/2);

    mSprite.setScale(-1.f, 1.f);
    setVelocity(b2Vec2(velocity, 0.f));
}

unsigned int BossSlime::getCategory() const{
    return Category::Enemy;
}

void BossSlime::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mSprite, states);
}

void BossSlime::updateCurrent(sf::Time deltaTime){

    //Move sprite according to Box2D's World step
    b2Vec2 physPos = mB2Body->GetPosition();
    sf::Vector2f previousRenderPos = mSprite.getPosition();
    float mapHeight = previousRenderPos.y + mPreviousPosition.y * 70.f;
    mSprite.setPosition(physPos.x * 70.f, mapHeight - physPos.y * 70.f);
    mPreviousPosition = physPos;
    mSprite.update(deltaTime);
}

void BossSlime::stopAnimation(){
    //Need this check because during world restarts
    //we may try to stop non-existant animations due
    //to player resetting
    if ((mSprite.getAnimation()->getSize() > 0) &&
        (mSprite.getAnimation()->getSize() < 10))
        mSprite.stop();
}