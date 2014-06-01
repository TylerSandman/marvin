#include <SFML/Graphics.hpp>
#include <json_spirit_reader_template.h>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "TiledJSONLoader.h"

TiledJSONLoader::TiledJSONLoader(const std::string &pathToMaps, const std::string &pathToTilesets):
        mPathToMaps(pathToMaps),
        mPathToTilesets(pathToTilesets),
        mMapWidth(0),
        mMapHeight(0),
        mTileWidth(0),
        mTileHeight(0),
        mMapLoaded(false){
}

sf::Vector2i TiledJSONLoader::getMapSize(){
    return sf::Vector2i(mMapWidth,mMapHeight);
}

sf::Vector2i TiledJSONLoader::getTileSize(){
    return sf::Vector2i(mTileWidth,mTileHeight);
}

tiled::Tilesets TiledJSONLoader::getTilesets(){
    return mTilesets;
}

tiled::TileLayers TiledJSONLoader::getTileLayers(){
    return mTileLayers;
}

tiled::ObjectGroups TiledJSONLoader::getObjectGroups(){
    return mObjectGroups;
}

bool TiledJSONLoader::isMapLoaded(){
    return mMapLoaded;
}

void TiledJSONLoader::load(const std::string &file){

    std::cout << "Loading map file" << std::endl;
    json_spirit::mObject mapObject = loadFile(file);
    std::cout << "Parsing map attributes" << std::endl;
    loadMapAttributes(mapObject);
    std::cout << "Parsing tilesets" << std::endl;
    loadTilesets(mapObject);
    std::cout << "Parsing layers" << std::endl;
    loadLayers(mapObject);
    std::cout << "Map loaded" << std::endl;
    mMapLoaded = true;
}

json_spirit::mObject TiledJSONLoader::loadFile(const std::string &file){

    std::string mapFile = mPathToMaps + file;
    std::ifstream is(mapFile.c_str());
    json_spirit::mValue value;
    if (!json_spirit::read_stream(is,value))
        throw std::runtime_error("Failed to read map file " + file);

    //Make sure our root json is an object, not array
    assert(value.type() == json_spirit::Value_type::obj_type);
    json_spirit::mObject object = value.get_obj();
    return object;
}

void TiledJSONLoader::loadMapAttributes(json_spirit::mObject &map){
    mMapWidth = map["width"].get_int();
    mMapHeight = map["height"].get_int();
    mTileWidth = map["tilewidth"].get_int();
    mTileHeight = map["tileheight"].get_int();
}

void TiledJSONLoader::loadTilesets(json_spirit::mObject &map){

    json_spirit::mArray tilesetsVal = map["tilesets"].get_array();
    for(auto &tilesetVal : tilesetsVal){
        json_spirit::mObject tilesetMap = tilesetVal.get_obj();
        tiled::Tileset currentTileset;
        currentTileset.name = tilesetMap["name"].get_str();
        currentTileset.file = tilesetMap["image"].get_str();
        currentTileset.firstGID = tilesetMap["firstgid"].get_int();
        currentTileset.imageHeight = tilesetMap["imageheight"].get_int();
        currentTileset.imageWidth = tilesetMap["imagewidth"].get_int();
        currentTileset.margin = tilesetMap["margin"].get_int();
        currentTileset.spacing = tilesetMap["spacing"].get_int();
        mTilesets.push_back(currentTileset);

        //Load our tileset
        sf::Texture texture;
        if (!texture.loadFromFile(mPathToTilesets + currentTileset.file))
            throw std::runtime_error("Tiled JSON Loader failed to read tileset " + currentTileset.file);
        mTilesetTextures[currentTileset.name] = texture;
    }
}

void TiledJSONLoader::loadLayers(json_spirit::mObject &map){

    json_spirit::mArray layers = map["layers"].get_array();
    for (auto &layerVal : layers){
        json_spirit::mObject layer = layerVal.get_obj();
        std::string type = layer["type"].get_str();
        if (type.compare("tilelayer") == 0)
            loadTileLayer(layer, mTilesets);
        if (type.compare("objectgroup") == 0)
            loadObjectGroup(layer, mTilesets);
    }
}

void TiledJSONLoader::loadTileLayer(json_spirit::mObject &layer, tiled::Tilesets &tilesets){

    tiled::TileLayer currentLayer;
    currentLayer.name = layer["name"].get_str();
    currentLayer.visible = layer["visible"].get_bool();
    currentLayer.opacity = layer["opacity"].get_real();
    currentLayer.tiles = loadTiles(layer, tilesets);
    mTileLayers.push_back(currentLayer);
}

tiled::TileGrid TiledJSONLoader::loadTiles(json_spirit::mObject &layer, tiled::Tilesets &tilesets){

    json_spirit::mArray tileArray = layer["data"].get_array();
    
    //Initial fill of empty tiles
    tiled::TileGrid layerGrid;
    for(int row = 0; row < mMapHeight; ++row){
        tiled::Tiles currentRow;
        for (int col = 0; col < mMapWidth; ++col){
            currentRow.push_back(tiled::Tile());
        }
        layerGrid.push_back(currentRow);
    }

    int mapTileNum = 0;
    int mapRow = 0;
    for(auto &tile : tileArray){
        tiled::Tile currentTile = loadTile(tile, tilesets);
        tiled::Tileset tileset = getTilesetFromGID(currentTile.GID, tilesets);
        currentTile.sprite.setPosition(
        static_cast<float>((mapTileNum % mMapWidth) * (mTileWidth) + tileset.margin),
        static_cast<float>((mapRow) * (mTileHeight) + tileset.margin));
        layerGrid[mapRow][mapTileNum % mMapWidth] = currentTile;
        if ((mapTileNum + 1) % mMapWidth == 0){
            ++mapRow;
        }
        ++mapTileNum; 
    }
    return layerGrid;
}

tiled::Tile TiledJSONLoader::loadTile(json_spirit::mValue &tileVal, tiled::Tilesets &tilesets){
    
    tiled::Tile currentTile;
    unsigned int GID = tileVal.get_int();
    currentTile.GID = GID;
    if (GID == 0) return currentTile;

    tiled::Tileset tileset = getTilesetFromGID(GID, tilesets);
    int tilesPerRow = static_cast<int>
        (std::floor(static_cast<float>
            (tileset.imageWidth-tileset.margin) / (mTileWidth + tileset.spacing)));
    int tilesetRow = static_cast<int>
        (std::ceil(static_cast<float>(GID-tileset.firstGID) / tilesPerRow));
    int tilesetCol = tilesPerRow * (1 - tilesetRow) + (GID-tileset.firstGID+1);
    sf::Vector2i spriteSheetPos(
        ((tilesetCol-1) * (mTileWidth + tileset.spacing) + tileset.margin - 1),
        ((tilesetRow-1) * (mTileHeight + tileset.spacing) + tileset.margin- 1));
    sf::Texture &tilesetTexture = mTilesetTextures[tileset.name];
    sf::Sprite tileSprite;
    tileSprite.setTexture(tilesetTexture);
    tileSprite.setTextureRect(sf::IntRect(spriteSheetPos, sf::Vector2i(mTileWidth+1, mTileHeight + 1)));
    currentTile.sprite = tileSprite;
    return currentTile;
}

void TiledJSONLoader::loadObjectGroup(json_spirit::mObject &group, tiled::Tilesets &tilesets){

    tiled::ObjectGroup currentGroup;
    currentGroup.name = group["name"].get_str();
    currentGroup.visible = group["visible"].get_bool();
    currentGroup.opacity = group["opacity"].get_real();
    currentGroup.objects = loadObjects(group, tilesets);
    mObjectGroups.push_back(currentGroup);
}

tiled::Objects TiledJSONLoader::loadObjects(json_spirit::mObject &group, tiled::Tilesets &tilesets){

    json_spirit::mArray objectArray = group["objects"].get_array();

    tiled::Objects objects;
    for(auto &object : objectArray){
        objects.push_back(loadObject(object, tilesets));
    }
    return objects;
}

tiled::Object TiledJSONLoader::loadObject(json_spirit::mValue &objectVal, tiled::Tilesets &tilesets){

    tiled::Object currentObject;
    json_spirit::mObject object = objectVal.get_obj();
    unsigned int GID;
    if (object.find("gid") == object.end())
        GID = 0;
    else
        GID = object["gid"].get_int();
    currentObject.GID = GID;
    currentObject.name = object["name"].get_str();
    currentObject.type = object["type"].get_str();
    currentObject.position = sf::Vector2f(
        static_cast<float>(object["x"].get_int()),
        static_cast<float>(object["y"].get_int()));
    if (GID == 0) return currentObject;

    tiled::Tileset tileset = getTilesetFromGID(GID, tilesets);
    int tilesPerRow = static_cast<int>
        (std::floor(static_cast<float>
            (tileset.imageWidth-tileset.margin) / (mTileWidth + tileset.spacing)));
    int tilesetRow = static_cast<int>
        (std::ceil(static_cast<float>(GID-tileset.firstGID) / tilesPerRow));
    int tilesetCol = tilesPerRow * (1 - tilesetRow) + (GID-tileset.firstGID+1);
    sf::Vector2i spriteSheetPos(
        ((tilesetCol-1) * (mTileWidth + tileset.spacing) + tileset.margin - 1),
        ((tilesetRow-1) * (mTileHeight + tileset.spacing) + tileset.margin) - 1);
    sf::Texture &tilesetTexture = mTilesetTextures[tileset.name];
    sf::Sprite objectSprite;
    objectSprite.setTexture(tilesetTexture);
    objectSprite.setTextureRect(sf::IntRect(spriteSheetPos, sf::Vector2i(mTileWidth+1, mTileHeight+1)));
    objectSprite.setPosition(sf::Vector2f(
        currentObject.position.x,
        currentObject.position.y - mTileHeight));
    currentObject.sprite = objectSprite;
    return currentObject;
}

tiled::Tileset TiledJSONLoader::getTilesetFromGID(unsigned int GID, tiled::Tilesets &tilesets){
    tiled::Tileset tileset;
    for(unsigned int i = 0; i < tilesets.size(); ++i){
        if ((i == tilesets.size() - 1)||((GID >= tilesets[i].firstGID)&&
                                         (GID < tilesets[i+1].firstGID))){
            return tilesets[i];
        }
    }
    return tilesets[0];
}