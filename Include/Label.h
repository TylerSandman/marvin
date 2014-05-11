#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "ResourceManager.h"

namespace GUI{

class Label : public Component{

public:
    typedef std::shared_ptr<Label> Ptr;

public:
    Label(const std::string &text, sf::Color color, FontManager &fontManager);
    virtual bool isSelectable();
    void setText(const std::string &text);
    virtual void handleEvent(const sf::Event &event);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Text mText;
};
};