#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.h"

namespace GUI{

class Container : public Component{

public:
    typedef std::shared_ptr<Container> Ptr;

public:
    Container(Component *parent=nullptr);
    virtual bool isSelectable();
    virtual void handleEvent(const sf::Event &event);
    virtual void add(Component::Ptr component);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

protected:
    std::vector<Component::Ptr> mChildren;
};
}