#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <cassert>
#include "SceneNode.h"

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

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states){
    
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states){}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states){

    for (auto &child : mChildren)
        child.get()->draw(target, states);
}