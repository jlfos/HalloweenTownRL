/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

ForestMapGenerator::ForestMapGenerator(){
	rng = TCODRandom::getInstance();
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
	TCODMap* emptyMap = new TCODMap(width, height);
	for (int tileX = 0; tileX < width; tileX++) {
		for (int tileY = 0; tileY < height; tileY++) {
			int rand = 0;
			if(tileX<=width/2)
				rand = rng->getInt(25, 100);
			else
				rand = rng->getInt(78, 100);

			int character = Actor::CharacterCodes::RAINBOW;
			TCODColor fog;
			TCODColor visible;
			int tileIndex = tileX+tileY*width;
			if(rand%100==0){
				emptyMap->setProperties(tileX, tileY, true, false);
				visible = TCODColor(139, 69, 19);
				fog = TCODColor(97, 49, 12);
				character = Actor::CharacterCodes::YEN_SYMBOL;
			}
			else{
				emptyMap->setProperties(tileX, tileY, true, true);
				visible = TCODColor::green;
				fog = TCODColor::darkerGreen;
				character = Actor::CharacterCodes::PERIOD;
			}
			map->SetTileProperties(tileIndex, visible, fog, character);
		}
	}
	return emptyMap;
}

void ForestMapGenerator::PopulateActors(Map* map){

}

void ForestMapGenerator::AddItem(Map* map, int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	map->actors.push(item);
}
