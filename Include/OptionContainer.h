#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.h"
#include "State.h"
#include "Option.h"

namespace GUI{

class OptionContainer : public Component{

public:
    typedef std::shared_ptr<OptionContainer> Ptr;

public:
    OptionContainer(State::Context context);
    virtual bool isSelectable();
    virtual void handleEvent(const sf::Event &event);
    virtual void add(Option::Ptr option);
    virtual void activate();
    virtual void deactivate();
    void apply();

protected:
    virtual bool hasSelection();
    virtual void select(std::size_t index);
    virtual void selectNext();
    virtual void selectPrevious();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  
protected:
    std::vector<Option::Ptr> mChildren;
    SoundPlayer &mSoundPlayer;   
    int mSelectedChild;
};
}