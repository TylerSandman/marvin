#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <map>
#include "Component.h"
#include "ResourceManager.h"

namespace GUI{

class Option : public Component{

public:
    typedef std::shared_ptr<Option> Ptr;
    typedef std::function<void(int)> Callback;

public:
    Option(std::string name, FontManager &fontManager);
    virtual void setCallback(Callback callback);
    virtual bool isSelectable();
    virtual void select();
    virtual void deselect();
    virtual bool hasSelection();
    void selectNext();
    void selectPrevious();
    void select(std::size_t index);
    virtual void activate();
    virtual void deactivate();
    virtual void handleEvent(const sf::Event &event);
    void addPair(std::pair<std::string, int> pair);

protected:
    Callback mCallback;
    int mSelectedChild;
    sf::Text mNameText;
    sf::Text mValueText;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::vector<std::pair<std::string, int>> mOptionPairs;
};
}