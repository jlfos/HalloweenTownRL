/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

TCODMap* MostlyEmptyMapGenerator::Generate(Map* map){
	TCODMap* emptyMap = new TCODMap(map->width, map->height);
	for (int tilex = 0; tilex < map->width; tilex++) {
		for (int tiley = 0; tiley < map->height; tiley++) {
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = rng->getInt(0, 200);
			if(rand%200==0)
				emptyMap->setProperties(tilex, tiley, true, false);
			else
				emptyMap->setProperties(tilex, tiley, true, true);


			if(rand%100==0)
				AddItem(map, tilex, tiley);
		}
	}
	return emptyMap;
}

void MostlyEmptyMapGenerator::AddItem(Map* map, int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	map->actors.push(item);
}
