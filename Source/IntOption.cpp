#include "IntOption.h"

namespace GUI{

IntOption::IntOption(std::string name, FontManager &fontManager) :
    Option(name, fontManager),
    mOptionPairs(){}

void IntOption::setCallback(Callback callback){
    mCallback = callback;
}

void IntOption::selectNext(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild + 1) % mOptionPairs.size();
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void IntOption::selectPrevious(){

    if (!hasSelection())
        return;

    mSelectedChild = (mSelectedChild + mOptionPairs.size() - 1) % mOptionPairs.size();
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void IntOption::select(std::size_t index){
    mSelectedChild = index; 
    mValueText.setString(mOptionPairs[mSelectedChild].first);
}

void IntOption::activate(){

    Component::activate();

    if (mCallback)
        mCallback(mOptionPairs[mSelectedChild].second);
    deactivate();
}

void IntOption::deactivate(){
    Component::deactivate();
}

void IntOption::addPair(std::pair<std::string, int> pair){

    mOptionPairs.push_back(pair);

    if (!hasSelection())
        select(mOptionPairs.size() - 1);
}
}