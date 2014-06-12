#include "Option.h"

namespace GUI{

Option::Option(std::string name, FontManager &fontManager) :
    mSelectedChild(-1),
    mNameText(name, fontManager.get(FontID::Main), 32),
    mValueText("", fontManager.get(FontID::Main), 32){
    
    mNameText.setColor(sf::Color::Black);
    sf::FloatRect bounds = mNameText.getGlobalBounds();
    mNameText.setOrigin(bounds.width/2, bounds.height/2);
    mNameText.move(-150.f, 0);

    mValueText.setColor(sf::Color::Black);
    sf::FloatRect valueBounds = mValueText.getGlobalBounds();
    mValueText.setOrigin(valueBounds.width/2, bounds.height/2);
    mValueText.move(150.f, 0);
}

bool Option::isSelectable(){
    return true;
}

void Option::select(){

    Component::select();
    mNameText.setColor(sf::Color::White);
    mValueText.setColor(sf::Color::White);
}

void Option::deselect(){

    Component::deselect();
    mNameText.setColor(sf::Color::Black);
    mValueText.setColor(sf::Color::Black);
}

bool Option::hasSelection(){
    return mSelectedChild >= 0;
}

void Option::handleEvent(const sf::Event &event){}

void Option::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mNameText, states);
    target.draw(mValueText, states);
}
}