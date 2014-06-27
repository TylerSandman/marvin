#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Command.h"

class SceneNode: public sf::Transformable, public sf::Drawable {

public:
    SceneNode();
    void onCommand(const Command &command, sf::Time deltaTime);
    virtual unsigned int getCategory() const;
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode &node);
    void update(sf::Time deltaTime);
    virtual void flagForRemoval();
    virtual bool isFlaggedForRemoval() const;
    virtual void remove();
    virtual void removeChildren();

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void updateChildren(sf::Time deltaTime);

private:
    std::vector<Ptr> mChildren;
    SceneNode *mParent;
    bool mFlaggedForRemoval;
};