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
    bool isSelectable();
    void handleEvent(const sf::Event &event);
    void add(Component::Ptr component);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::vector<Component::Ptr> mChildren;
};
}