#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include "Entity.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"

class Marvin : public Entity{

public:
    enum FacingDirection{
        Left,
        Right
    };

    enum AnimationID{
        None,
        Walk,
        Jump,
        Hurt
    };
    
public:
    Marvin(TextureManager &textureManager, b2Body *playerBody);
    unsigned int getCategory();
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    sf::Vector2f getRenderPosition();
    void setRenderPosition(sf::Vector2f position);
    b2Vec2 getPosition();
    void setVelocity(b2Vec2 velocity);
    b2Vec2 getVelocity(); 
    sf::FloatRect getBoundingBox();
    bool isOnGround();
    void setNumFootContacts(int num);
    int getNumFootContacts();
    void turn(FacingDirection direction);
    void stopAnimation();

private:
    b2Body *mB2Body;
    AnimatedSprite mSprite;
    FacingDirection mCurrentFacingDirection;
    std::map<AnimationID, Animation> mAnimationMap;
    b2Vec2 mPreviousPosition;
    int numFootContacts; //Ew. So...hacky. Gotta fix this.
};