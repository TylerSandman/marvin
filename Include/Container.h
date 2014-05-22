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
    Container(sf::View view);
    virtual bool isSelectable();
    virtual void handleEvent(const sf::Event &event);
    virtual void add(Component::Ptr component);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    bool hasSelection();
    void select(std::size_t index);
    void selectNext();
    void selectPrevious();

protected:
    std::vector<Component::Ptr> mChildren;
    int mSelectedChild;
    sf::View mView;
};
}