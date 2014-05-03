#pragma once
#include <SFML\Graphics.hpp>
#include <map>
#include <memory>
#include <string>

enum TextureID{
    Background1,
    PlayerStanding
};

template <typename Identifier, typename Resource>
class ResourceManager {

public:
    void load(Identifier id, const std::string &file);
    Resource& get(Identifier id);
private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

//Typedefs for ease of use

typedef ResourceManager<TextureID, sf::Texture> TextureManager;

//Function Definitions

template <typename Identifier, typename Resource>
void ResourceManager<Identifier, Resource>::load(Identifier id, const std::string &file){

    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(file)){
        throw std::runtime_error("Resource Manager could not load " + file);
    }
    mResourceMap[id] = std::move(resource);
}

template <typename Identifier, typename Resource>
Resource& ResourceManager<Identifier, Resource>::get(Identifier id){

    auto resource = mResourceMap.find(id);
    assert(resource != mResourceMap.end());
    return *resource->second;
}