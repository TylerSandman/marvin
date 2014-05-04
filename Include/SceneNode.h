#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class SceneNode: public sf::Transformable, public sf::Drawable {

public:
    SceneNode();
    typedef std::unique_ptr<SceneNode> Ptr;

public:
    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode &node);
    void update(sf::Time deltaTime);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time deltaTime);
    void updateChildren(sf::Time deltaTime);

private:
    std::vector<Ptr> mChildren;
    SceneNode *mParent;
};