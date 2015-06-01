/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

ForestMapGenerator::ForestMapGenerator() : ForestMapGenerator(MapGenerator::Orientation::NORTH){

}

ForestMapGenerator::ForestMapGenerator(MapGenerator::Orientation orientation){
	rng = TCODRandom::getInstance();
	this->orientation = orientation;
}

ForestMapGenerator::~ForestMapGenerator(){
	if(rng != nullptr){
		delete rng;
		rng = nullptr;
	}
}

/**
 * Generates the tiles for the map
 */
TCODMap* ForestMapGenerator::Generate(Map* map, bool generateActors){

	int width = map->GetWidth();
	int height = map->GetHeight();
	TCODMap* forestMap = new TCODMap(width, height);
	for (int tileX = 0; tileX < width; tileX++) {
			for (int tileY = 0; tileY < height; tileY++) {
					GenerateTile(tileX, tileY, width, height, forestMap, map);
			}
	}


	return forestMap;
}

void ForestMapGenerator::PopulateActors(Map* map){

}

/**
 * Generates specific tiles for the map
 */
void ForestMapGenerator::GenerateTile(int x, int y, int width, int height, TCODMap* forestMap, Map* map){
	bool lowTreeChance = true;
	switch(orientation){
		case MapGenerator::Orientation::EAST:
			lowTreeChance = x<=width/2;
			break;
		case MapGenerator::Orientation::WEST:
			lowTreeChance = x>=width/2;
			break;
		case MapGenerator::Orientation::NORTH:
			lowTreeChance = y>=height/2;
			break;
		case MapGenerator::Orientation::SOUTH:
			lowTreeChance = y<=height/2;
			break;
	}

	int rand = 0;
	if(lowTreeChance)
		rand = rng->getInt(80, 100);
	else
		rand = rng->getInt(90, 100);
	if(rand%100==0 && x!=0 && y!=0 && x!=width-1 && y!=height-1)
		GenerateTree(x, y, width, forestMap, map);
	else
		GenerateGrass(x, y, width, forestMap, map);

}

/**
 * Generates Tree tile
 */
void ForestMapGenerator::GenerateTree(int x, int y, int width, TCODMap* forestMap, Map* map){
	int tileIndex = x+y*width;
	forestMap->setProperties(x, y, false, false);
	TCODColor visible = TCODColor(139, 69, 19);
	TCODColor fog = TCODColor(97, 49, 12);
	int character = Actor::CharacterCodes::YEN_SYMBOL;
	map->SetTileProperties(tileIndex, visible, fog, character);
}

/**
 *  Generates Grass tile
 */
void ForestMapGenerator::GenerateGrass(int x, int y, int width, TCODMap* forestMap, Map* map){
	int tileIndex = x+y*width;
	forestMap->setProperties(x, y, true, true);
	TCODColor visible = TCODColor::green;
	TCODColor fog = TCODColor::darkerGreen;
	int character = Actor::CharacterCodes::PERIOD;
	map->SetTileProperties(tileIndex, visible, fog, character);
}

