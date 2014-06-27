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

    enum Type{
        Normal,
        Pressed,
        Disabled
    };

public:
    Button(TextureManager &textureManager); //Button without text
    Button(TextureManager &textureManager, FontManager &fontManager);
    virtual void setCallback(Callback callback);
    virtual void setToggle(bool toggle);
    virtual bool isSelectable();
    virtual void enable();
    virtual void disable();
    virtual void select();
    virtual void deselect();
    virtual void activate();
    virtual void deactivate();
    virtual void handleEvent(const sf::Event &event);
    void setText(const std::string &text, sf::Color color);

protected:
    sf::IntRect mTextureNormalRect;
    sf::IntRect mTexturePressedRect;
    sf::IntRect mTextureDisabledRect;  
    Callback mCallback;
    sf::Sprite mSprite;
    bool mIsToggle;
    bool mEnabled;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void changeTexture(Type buttonType);

private:   
    sf::Text mText; //Base button exclusive

};
}