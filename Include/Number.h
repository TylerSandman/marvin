#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include "Component.h"

namespace GUI{
class Number : public Component{

public:
    typedef std::shared_ptr<Number> Ptr;

public:
    Number(TextureManager &textureManager, unsigned int num);
    virtual void handleEvent(const sf::Event &event);
    virtual bool isSelectable();
    
private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    sf::IntRect getRectFromNum(unsigned int num);

private:
    sf::Sprite mOnesSprite;
    sf::Sprite mTensSprite;
};
}