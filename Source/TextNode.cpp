#pragma once
#include <SFML\Graphics.hpp>
#include "TextNode.h"

TextNode::TextNode(const sf::Font &font){
    mText.setFont(font);
}

TextNode::TextNode(const sf::Font &font, const std::string &text){

    mText.setFont(font);
    mText.setString(text);
}

void TextNode::setText(const std::string &text){
    mText.setString(text);
}

void TextNode::setColor(sf::Color color){
    mText.setColor(color);
}

void TextNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{
    target.draw(mText, states);
}