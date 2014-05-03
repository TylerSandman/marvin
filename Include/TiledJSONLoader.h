#pragma once
#include <SFML/Graphics.hpp>
#include <json_spirit_reader_template.h>
#include "Tileset.h"
#include "TileLayer.h"
#include "ObjectGroup.h"
#include <string>
#include <vector>

class TiledJSONLoader{

public:
	TiledJSONLoader(const std::string &pathToMaps, const std::string &pathToTilesets);

public:
	void load(const std::string &file);
	sf::Vector2i getMapSize();
	sf::Vector2i getTileSize();
	tiled::Tilesets getTilesets();
	tiled::TileLayers getTileLayers();
	tiled::ObjectGroups getObjectGroups();
    bool isMapLoaded();

private:
	std::string mPathToMaps;
    std::string mPathToTilesets;
	bool mMapLoaded;
	int mMapWidth;	//In tiles
	int mMapHeight;	//In tiles
	int mTileWidth;
	int mTileHeight;
	tiled::Tilesets mTilesets;
	tiled::TileLayers mTileLayers;
	tiled::ObjectGroups mObjectGroups;
	std::map<std::string, sf::Texture> mTilesetTextures;
	//json_spirit::mArray mImageLayers;

private:
	json_spirit::mObject loadFile(const std::string &file);
	void loadMapAttributes(json_spirit::mObject &map);
	void loadTilesets(json_spirit::mObject &map);
	void loadLayers(json_spirit::mObject &map);
	void loadTileLayer(json_spirit::mObject &layer, tiled::Tilesets &tilesets);
	tiled::TileGrid loadTiles(json_spirit::mObject &layer, tiled::Tilesets &tilesets);
	tiled::Tile loadTile(json_spirit::mValue &tileVal, tiled::Tilesets &tilesets);
	void loadObjectGroup(json_spirit::mObject &group, tiled::Tilesets &tilesets);
	tiled::Objects loadObjects(json_spirit::mObject &group, tiled::Tilesets &tilesets);
	tiled::Object loadObject(json_spirit::mValue &objectVal, tiled::Tilesets &tilesets);
	tiled::Tileset getTilesetFromGID(unsigned int GID, tiled::Tilesets &tilesets);
};