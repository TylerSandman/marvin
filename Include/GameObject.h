#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include "SceneNode.h"
#include "Command.h"
#include "Object.h"

class GameObject : public SceneNode{

public:
    typedef std::unique_ptr<GameObject> Ptr;
    GameObject(Category::Type category, sf::Sprite &sprite, b2Body *objectBody);
    virtual unsigned int getCategory();
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Category::Type mCategory;
    sf::Sprite mSprite;
    b2Body *mBody;   
};