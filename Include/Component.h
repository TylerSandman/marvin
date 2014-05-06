#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
namespace GUI{

class Component : public sf::Drawable, public sf::Transformable{

public:
    typedef std::shared_ptr<Component> Ptr;

public:
    Component(Component *parent=nullptr);
    virtual ~Component();
    virtual bool isSelectable() = 0;
    bool isSelected() const;
    virtual void select();
    virtual void deselect();
    virtual void handleEvent(const sf::Event &event) = 0;

    //Used for mouse click events
    sf::Vector2f getAbsolutePosition() const;
    sf::Transform getAbsoluteTransform() const;

private:
    bool mIsSelected;
    Component *mParent;
};
}