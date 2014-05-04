#pragma once
#include <SFML\Graphics.hpp>
#include "SceneNode.h"

class SpriteNode : public SceneNode{

public:
    explicit SpriteNode(const sf::Texture &texture);
    explicit SpriteNode(const sf::Texture &texture, const sf::IntRect &rect);

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;
};