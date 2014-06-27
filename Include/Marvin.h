#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include "Entity.h"
#include "ResourceManager.h"
#include "AnimatedSprite.h"
#include "GrassPlatform.h"

class Marvin : public Entity{

public:
    enum AnimationID{
        None,
        Walk,
        Jump,
        Hurt,
        Fade
    };
    
public:
    Marvin(TextureManager &textureManager, b2Body *playerBody);
    unsigned int getCategory() const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void setControlledVelocity(b2Vec2 velocity);
    b2Vec2 getControlledVelocity();
    void attachPlatform(GrassPlatform *platform);
    void detachPlatform();
    GrassPlatform* getAttachedPlatform() const;
    bool isOnGround() const;
    bool isFaded() const;
    void setNumFootContacts(int num);
    int getNumFootContacts();
    void turn(FacingDirection direction);
    void stopAnimation();
    void fade();

private:
    void debugDraw(sf::RenderTarget &target, sf::RenderStates states) const;
    Entity::FacingDirection mCurrentFacingDirection;
    std::map<AnimationID, Animation> mAnimationMap;
    GrassPlatform *mAttachedPlatform;
    b2Vec2 mControlledVelocity;
    bool mFading;
    int numFootContacts; //Ew. So...hacky. Gotta fix this.
};