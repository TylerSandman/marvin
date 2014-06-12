#include "VectorOption.h"

namespace GUI{

VectorOption::VectorOption(std::string name, FontManager &fontManager) :
    Option(name, fontManager),
    mOptionPairs(){}

void VectorOption::setCallback(Callback callback){
    mCallback = callback;
}

void VectorOption::selectNext(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild + 1) % mOptionPairs.size();
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void VectorOption::selectPrevious(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild - 1) % mOptionPairs.size();
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void VectorOption::select(std::size_t index){
    mSelectedChild = index; 
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void VectorOption::activate(){

    Component::activate();

    if (mCallback)
        mCallback(mOptionPairs[mSelectedChild].second);
    deactivate();
}

void VectorOption::deactivate(){
    Component::deactivate();
}

void VectorOption::addPair(std::pair<std::string, sf::Vector2f> pair){

    mOptionPairs.push_back(pair);

    if (!hasSelection())
        select(mOptionPairs.size() - 1);
}
}