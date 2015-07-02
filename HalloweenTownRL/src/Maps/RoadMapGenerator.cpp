/*
 * RoadMapGenerator.cpp
 *
 *  Created on: Feb 28, 2015
 */
#include "libtcod.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"
#include "RoadMapGenerator.hpp"
#include "../Tile/TileColors.hpp"


RoadMapGenerator::RoadMapGenerator():RoadMapGenerator(MapGenerator::Orientation::NORTH) {

}
RoadMapGenerator::RoadMapGenerator(MapGenerator::Orientation orientation){
	this->orientation = orientation;
}

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	int width = map->GetWidth();
	int height = map->GetHeight();
	TCODMap* roadMap = new TCODMap(width, height);

	for (int tileX = 0; tileX < width; tileX++) {
		for (int tileY = 0; tileY < height-1; tileY++) {
			bool roadFlag = false;
			switch(orientation){
				case MapGenerator::Orientation::NORTH:
				case MapGenerator::Orientation::SOUTH:
					roadFlag = tileX <= (width/2)+3  &&  tileX >= (width/2)-3;
					break;
				case MapGenerator::Orientation::EAST:
				case MapGenerator::Orientation::WEST:
					roadFlag = tileY <= (height/2)+3  &&  tileY >= (height/2)-3;
					break;
			}

			if(roadFlag)
				GenerateRoad(tileX, tileY, width, roadMap, map);
			else
				GenerateGrass(tileX, tileY, width, roadMap, map);


		}
	}
	return roadMap;
}


void RoadMapGenerator::PopulateActors(Map* map){

}

void RoadMapGenerator::GenerateGrass(int x, int y, int width, TCODMap* roadMap, Map* map){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::green;
	TCODColor fog = TileColors::darkerGreen;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, fog, character);
}


void RoadMapGenerator::GenerateRoad(int x, int y, int width, TCODMap* roadMap, Map* map){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::lightGrey;
	TCODColor fog = TileColors::darkGrey;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, fog, character);
}
