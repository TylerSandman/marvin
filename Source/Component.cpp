#include "Component.h"
namespace GUI{

Component::Component(Component *parent) : 
    mIsSelected(false),
    mParent(parent){}

Component::~Component(){}

bool Component::isSelected() const{ 
    return mIsSelected; 
}

void Component::select(){
    mIsSelected = true;
}

void Component::deselect(){
    mIsSelected = false;
}

sf::Vector2f Component::getAbsolutePosition() const{
    return getAbsoluteTransform() * sf::Vector2f();
}

sf::Transform Component::getAbsoluteTransform() const{

    sf::Transform transform = sf::Transform::Identity;

    for (const Component *comp = this; comp != nullptr; comp = comp->mParent)
        transform = comp->getTransform() * transform;

    return transform;
}
}