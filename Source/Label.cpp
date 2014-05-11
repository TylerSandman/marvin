#include <SFML\Graphics.hpp>
#include "Label.h"


namespace GUI
{

Label::Label(const std::string& text, sf::Color color, FontManager& fontManager) : 
    mText(text, fontManager.get(FontID::Main), 16){

    mText.setColor(color);
    sf::FloatRect bounds = mText.getLocalBounds();
    mText.setOrigin(bounds.width/2, bounds.height/2);
}

bool Label::isSelectable(){
    return false;
}

void Label::handleEvent(const sf::Event&){}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();
    target.draw(mText, states);
}

void Label::setText(const std::string& text){
    mText.setString(text);
}
}