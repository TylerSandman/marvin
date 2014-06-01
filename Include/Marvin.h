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
    bool isOnGround();
    void setNumFootContacts(int num);
    int getNumFootContacts();
    void turn(FacingDirection direction);
    void stopAnimation();

private:
    FacingDirection mCurrentFacingDirection;
    std::map<AnimationID, Animation> mAnimationMap;
    int numFootContacts; //Ew. So...hacky. Gotta fix this.
};