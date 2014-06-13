#include "BoolOption.h"

namespace GUI{

BoolOption::BoolOption(std::string name, FontManager &fontManager) :
    Option(name, fontManager),
    mOptionPairs(){}

void BoolOption::setCallback(Callback callback){
    mCallback = callback;
}

void BoolOption::selectNext(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild + 1) % mOptionPairs.size();
    select(mSelectedChild);
}

void BoolOption::selectPrevious(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild - 1) % mOptionPairs.size();
    select(mSelectedChild);
}

void BoolOption::select(std::size_t index){
    mSelectedChild = index; 
    mValueText.setString(mOptionPairs[mSelectedChild].first);
    mValueText.setOrigin(
        mValueText.getGlobalBounds().width / 2.f,
        mNameText.getGlobalBounds().height / 2.f);
}

void BoolOption::activate(){

    Component::activate();

    if (mCallback)
        mCallback(mOptionPairs[mSelectedChild].second);
    deactivate();
}

void BoolOption::deactivate(){
    Component::deactivate();
}

void BoolOption::addPair(std::pair<std::string, bool> pair){

    mOptionPairs.push_back(pair);

    if (!hasSelection())
        select(mOptionPairs.size() - 1);
}
}