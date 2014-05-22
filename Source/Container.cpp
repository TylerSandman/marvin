#include <SFML\Graphics.hpp>
#include "Container.h"
#include <vector>

namespace GUI{

Container::Container(sf::View view) : 
    mChildren(),
    mSelectedChild(-1),
    mView(view),
    startViewPos(view.getCenter()){}

bool Container::isSelectable(){
    return false;
}

void Container::handleEvent(const sf::Event &event){

    if (hasSelection() && mChildren[mSelectedChild]->isActive()){
        mChildren[mSelectedChild]->handleEvent(event);
    }
    else if (event.type == sf::Event::KeyReleased){
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up){
            selectPrevious();
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down){
            selectNext();
        }
        else if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return){
            if (hasSelection())
                mChildren[mSelectedChild]->activate();
        }
    }
}

void Container::add(Component::Ptr component){
    mChildren.push_back(component);

    if (!hasSelection() && component->isSelectable())
        select(mChildren.size() - 1);
}

void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const{

    states.transform *= getTransform();
    target.setView(mView);
    for(auto &component : mChildren){
        target.draw(*component, states);
    }
}

bool Container::hasSelection(){
    return mSelectedChild >= 0;
}

void Container::select(std::size_t index){
    
    if (mChildren[index]->isSelectable()){
        if (hasSelection())
            mChildren[mSelectedChild]->deselect();
        mChildren[index]->select();
        mSelectedChild = index;
    }
}

void Container::selectNext(){

    if (!hasSelection())
        return;

    int next = mSelectedChild;
    do{
        next = (next + 1) % mChildren.size();
    } while (!mChildren[next]->isSelectable());

    int eleYPos = mChildren[next]->getPosition().y;
    int viewBottom = mView.getCenter().y + mView.getSize().y/2.f;
    if (eleYPos + 50 > viewBottom){
        mView.move(0.f, (next - mSelectedChild) * 100.f);
    }

    //Wrap around
    else if (next == 0){
        mView.setCenter(startViewPos);
    }
    select(next);
}

void Container::selectPrevious(){

    if (!hasSelection())
        return;

    int prev = mSelectedChild;
    do{
        prev = (prev + mChildren.size() - 1) % mChildren.size();
    } while (!mChildren[prev]->isSelectable());

    int eleYPos = mChildren[prev]->getPosition().y;
    int viewTop = mView.getCenter().y - mView.getSize().y/2.f;
    int viewBottom = mView.getCenter().y + mView.getSize().y/2.f;
    if (eleYPos - 50 < viewTop){
        mView.move(0.f, (prev - mSelectedChild) * 100.f);
    }

    //Wrap around
    //Assumes all non-selectable elements are adjacent and at the end of container
    else if (eleYPos + 50 > viewBottom){
        float viewOffset = 0.f;
        int currentChildIndex = 0;
        while (mChildren[currentChildIndex]->isSelectable()){
            if (mChildren[currentChildIndex]->getPosition().y + 50 > viewBottom){
                viewOffset += 100.f;
            }
            currentChildIndex++;
        }
        mView.move(0.f, viewOffset);
    }
    select(prev);
}
}
