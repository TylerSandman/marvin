#include <SFML\Graphics.hpp>
#include "Container.h"
#include <vector>

namespace GUI{

Container::Container(Component *parent) : 
    Component(parent),
    mChildren(){}

bool Container::isSelectable(){
    return false;
}

void Container::handleEvent(const sf::Event &event){

    for(auto &component : mChildren){
        component.get()->handleEvent(event);
    }
}

void Container::add(Component::Ptr component){
    mChildren.push_back(component);
}

void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    for(auto &component : mChildren){
        target.draw(*component, states);
    }
}
}