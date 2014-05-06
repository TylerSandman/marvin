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
    Button(TextureManager &textureManager, FontManager &fontManager, Component *parent=nullptr);
    void setOnClick(Callback callback);
    void setText(const std::string &text, sf::Color color);
    void setToggle(bool toggle);
    virtual bool isSelectable();
    virtual void handleEvent(const sf::Event &event);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    sf::FloatRect getRect() const;

private:
    Callback mOnClick;
    sf::Texture &mTexture;
    sf::Sprite mSprite;
    sf::Text mText;
    sf::Color mDefaultColor;
    bool mIsToggle;
};
}