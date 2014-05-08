#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <cassert>
#include "SceneNode.h"
#include "Command.h"

SceneNode::SceneNode(): 
    mChildren(),
    mParent(nullptr){}

unsigned int SceneNode::getCategory(){
    return Category::None;
}

void SceneNode::onCommand(const Command &command, sf::Time deltaTime){
    if (command.category & getCategory())
        command.action(*this, deltaTime);

    for(auto &child : mChildren){
        child.get()->onCommand(command, deltaTime);
    }
}

void SceneNode::attachChild(Ptr child){
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode &node){

    auto toDetach = std::find_if(mChildren.begin(), mChildren.end(),
        [&] (Ptr &ptr) -> bool { return ptr.get() == &node; });
    assert(toDetach != mChildren.end());
    Ptr found = std::move(*toDetach);
    found->mParent = nullptr;
    return found;
}

void SceneNode::update(sf::Time deltaTime){
    updateCurrent(deltaTime);
    updateChildren(deltaTime);
}

void SceneNode::updateCurrent(sf::Time deltaTime){}

void SceneNode::updateChildren(sf::Time deltaTime){
    for (auto &child : mChildren){
        child.get()->update(deltaTime);
    }
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const{}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const{

    for (auto &child : mChildren)
        child.get()->draw(target, states);
}