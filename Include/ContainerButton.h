#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include "Button.h"
#include "ResourceManager.h"

namespace GUI{

class ContainerButton : public Button{

public:
    typedef std::shared_ptr<ContainerButton> Ptr;

public:
    ContainerButton(TextureManager &textureManager);
    virtual bool isSelectable();
    virtual void enable();
    virtual void disable();
    virtual void select();
    virtual void deselect();
    virtual void activate();
    virtual void deactivate();
    virtual void add(Component::Ptr component);
    virtual void handleEvent(const sf::Event &event);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void changeTexture(Type buttonType);

private:
    sf::Sprite mPointer;
    std::vector<Component::Ptr> mChildren;
};
}