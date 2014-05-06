#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include "Component.h"
#include "ResourceManager.h"

namespace GUI{

class Button : public Component{

public:
    typedef std::shared_ptr<Button> Ptr;
    typedef std::function<void()> Callback;

public:
    Button(TextureManager &textureManager, Component *parent=nullptr);
    void setCallback(Callback callback);
    void setText(const std::string &text);
    void setToggle(bool toggle);
    virtual bool isSelectable();
    virtual void select();
    virtual void deselect();
    virtual void handleEvent(const sf::Event &event);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    Callback mCallback;
    sf::Texture &mTexture;
    sf::Sprite mSprite;
    sf::Text mText;
    bool mIsToggle;
};
}