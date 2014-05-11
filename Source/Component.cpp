#include "Component.h"
namespace GUI{

Component::Component() : 
    mIsSelected(false),
    mIsActive(false){}

Component::~Component(){}

bool Component::isSelected() const{ 
    return mIsSelected; 
}

void Component::select(){
    mIsSelected = true;
}

void Component::deselect(){
    mIsSelected = false;
}

void Component::activate(){
    mIsActive = true;
}

void Component::deactivate(){
    mIsActive = false;
}

bool Component::isActive(){
    return mIsActive;
}
}