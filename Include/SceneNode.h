#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class SceneNode: public sf::Transformable, public sf::Drawable {

public:
    typedef std::unique_ptr<SceneNode> Ptr;

public:
    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode &node);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states);
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states);
    virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states);

private:
    std::vector<Ptr> mChildren;
    SceneNode *mParent;
};