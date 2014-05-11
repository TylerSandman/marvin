#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include "Component.h"

namespace GUI{
class Image : public Component{

public:
    typedef std::shared_ptr<Image> Ptr;

public:
    Image(sf::Texture &texture, sf::IntRect textureRect=sf::IntRect(0,0,0,0));
    virtual void handleEvent(const sf::Event &event);
    virtual bool isSelectable();
    
private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Texture &mTexture;
    sf::Sprite mSprite;
};
}