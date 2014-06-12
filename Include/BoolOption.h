#pragma once
#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <vector>
#include "Option.h"
#include "ResourceManager.h"

namespace GUI{

class BoolOption : public Option{

public:
    typedef std::shared_ptr<BoolOption> Ptr;
    typedef std::function<void(bool)> Callback;

public:
    BoolOption(std::string name, FontManager &fontManager);
    void setCallback(Callback callback);
    virtual void selectNext();
    virtual void selectPrevious();
    virtual void select(std::size_t index);
    virtual void activate();
    virtual void deactivate();
    void addPair(std::pair<std::string, bool> pair);

private:
    Callback mCallback;
    std::vector<std::pair<std::string, bool>> mOptionPairs;

};
}