/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

TCODMap* ForestMapGenerator::Generate(Map* map, bool generateActors){
	int width = map->GetWidth();
	int height = map->GetHeight();
	TCODMap* emptyMap = new TCODMap(width, height);
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = 0;
			if(tilex<=width/2)
				rand = rng->getInt(25, 100);
			else
				rand = rng->getInt(78, 100);

			if(rand%100==0){
				emptyMap->setProperties(tilex, tiley, true, false);
				map->tiles[tilex+tiley*width].visibleColor = TCODColor(139, 69, 19);
				map->tiles[tilex+tiley*width].fogColor = TCODColor(97, 49, 12);
				map->tiles[tilex+tiley*width].character = Actor::CharacterCodes::YEN_SYMBOL;
			}
			else{
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*width].visibleColor = TCODColor::green;
				map->tiles[tilex+tiley*width].fogColor = TCODColor::darkerGreen;
				map->tiles[tilex+tiley*width].character = Actor::CharacterCodes::PERIOD;
			}
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
