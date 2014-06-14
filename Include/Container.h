#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.h"
#include "State.h"

namespace GUI{

class Container : public Component{

public:
    typedef std::shared_ptr<Container> Ptr;

public:
    Container(State::Context context, sf::View view);
    virtual bool isSelectable();
    virtual void handleEvent(const sf::Event &event);
    virtual void add(Component::Ptr component);
    void setView(sf::View view);
    sf::View getView();

protected:
    virtual bool hasSelection();
    virtual void select(std::size_t index);
    virtual void selectNext();
    virtual void selectPrevious();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  
protected:
    SoundPlayer &mSoundPlayer;
    std::vector<Component::Ptr> mChildren;
    int mSelectedChild;
    sf::View mView;
};
}