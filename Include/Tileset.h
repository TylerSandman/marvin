#pragma once
#include <vector>
#include <string>

namespace tiled{

	struct Tileset{

	public:
		unsigned int firstGID;
		std::string file;
		std::string name;
		int imageHeight;
		int imageWidth;
		int margin;
		int spacing;
	};

	typedef std::vector<Tileset> Tilesets;
}