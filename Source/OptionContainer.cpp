#include <SFML\Graphics.hpp>
#include "OptionContainer.h"
#include <vector>

namespace GUI{

OptionContainer::OptionContainer(State::Context context) : 
    mSoundPlayer(*context.soundPlayer),
    mSelectedChild(-1){}

bool OptionContainer::isSelectable(){
    return true;
}

void OptionContainer::handleEvent(const sf::Event &event){

    if (hasSelection() && mChildren[mSelectedChild]->isActive()){
        mChildren[mSelectedChild]->handleEvent(event);
    }
    else if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up){
            mSoundPlayer.play(SoundEffectID::Switch);
            selectPrevious();           
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down){
            mSoundPlayer.play(SoundEffectID::Switch);
            selectNext();          
        }
        else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left){
            mSoundPlayer.play(SoundEffectID::Switch);
            mChildren[mSelectedChild]->selectPrevious();
        }
        else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right){
            mSoundPlayer.play(SoundEffectID::Switch);
            mChildren[mSelectedChild]->selectNext();
        }
    }
}

void OptionContainer::add(Option::Ptr option){
    mChildren.push_back(option);

    if (!hasSelection() && option->isSelectable())
        select(mChildren.size() - 1);
}

void OptionContainer::activate(){
    Component::activate();
}

void OptionContainer::deactivate(){
    Component::deactivate();
}

void OptionContainer::apply(){
    for (auto &option : mChildren){
        option->activate();
    }
}

void OptionContainer::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    for(auto &component : mChildren){
        target.draw(*component, states);
    }
}

bool OptionContainer::hasSelection(){
    return mSelectedChild >= 0;
}

void OptionContainer::select(std::size_t index){
    
    if (mChildren[index]->isSelectable()){
        if (hasSelection())
            mChildren[mSelectedChild]->deselect();
        mChildren[index]->select();
        mSelectedChild = index;
    }
}

void OptionContainer::selectNext(){

    if (!hasSelection())
        return;

    int next = mSelectedChild;
    do{
        next = (next + 1) % mChildren.size();
    } while (!mChildren[next]->isSelectable());
    select(next);
}

void OptionContainer::selectPrevious(){

    if (!hasSelection())
        return;

    int prev = mSelectedChild;
    do{
        prev = (prev + mChildren.size() - 1) % mChildren.size();
    } while (!mChildren[prev]->isSelectable());
    select(prev);
}
}
