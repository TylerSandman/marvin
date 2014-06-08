#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include "Container.h"

namespace GUI{
class Panel : public Container{

public:
    typedef std::shared_ptr<Panel> Ptr;

public:
    Panel(State::Context context, sf::View view);
    
private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Texture &mTexture;
    sf::Sprite mSprite;
};
}