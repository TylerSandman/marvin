#include "Option.h"

namespace GUI{

Option::Option(std::string name, FontManager &fontManager) : 
    mOptionPairs(),
    mSelectedChild(-1),
    mNameText(name, fontManager.get(FontID::Main), 32),
    mValueText("", fontManager.get(FontID::Main), 32){
    
    mNameText.setColor(sf::Color::Black);
    sf::FloatRect bounds = mNameText.getGlobalBounds();
    mNameText.setOrigin(bounds.width/2, bounds.height/2);
    mNameText.move(-200.f, 0);

    mValueText.setColor(sf::Color::Black);
    sf::FloatRect valueBounds = mValueText.getGlobalBounds();
    mValueText.setOrigin(valueBounds.width/2, bounds.height/2);
    mValueText.move(200.f, 0);
}

void Option::setCallback(Callback callback){
    mCallback = callback;
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

void Option::selectNext(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild + 1) % mOptionPairs.size();
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void Option::selectPrevious(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild - 1) % mOptionPairs.size();
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void Option::select(std::size_t index){
    mSelectedChild = index; 
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void Option::activate(){

    Component::activate();

    if (mCallback)
        mCallback(mOptionPairs[mSelectedChild].second);
    deactivate();
}

void Option::deactivate(){
    Component::deactivate();
}

void Option::handleEvent(const sf::Event &event){}

void Option::addPair(std::pair<std::string, int> pair){

    mOptionPairs.push_back(pair);

    if (!hasSelection())
        select(mOptionPairs.size() - 1);
}

void Option::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.draw(mNameText, states);
    target.draw(mValueText, states);
}
}