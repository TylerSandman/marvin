#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
namespace GUI{

class Component : public sf::Drawable, public sf::Transformable{

public:
    typedef std::shared_ptr<Component> Ptr;

public:
    Component();
    virtual ~Component();
    virtual bool isSelectable() = 0;
    bool isSelected() const;
    virtual void select();
    virtual void deselect();
    virtual bool isActive();
    virtual void activate();
    virtual void deactivate();
    virtual void handleEvent(const sf::Event &event) = 0;

private:
    bool mIsSelected;
    bool mIsActive;
};
}