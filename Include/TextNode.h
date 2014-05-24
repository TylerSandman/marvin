#pragma once
#include <SFML\Graphics.hpp>
#include "SceneNode.h"

class TextNode : public SceneNode{

public:
    explicit TextNode(const sf::Font &font);
    explicit TextNode(const sf::Font &font, const std::string &text);
    void setText(const std::string &text);
    void setColor(sf::Color color);

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Text mText;
};