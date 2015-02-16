/*
 * EmptyMapGenerator.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: josh
 */
#include "../main.hpp"


TCODMap* EmptyMapGenerator::Generate(Map* map){
	TCODMap* emptyMap = new TCODMap(map->width, map->height);
	for (int tilex = 0; tilex < map->width; tilex++) {
		for (int tiley = 0; tiley < map->height; tiley++) {
			emptyMap->setProperties(tilex, tiley, true, true);
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = rng->getInt(0, 200);
			if(rand%100==0)
				AddItem(map, tilex, tiley);
		}
	}
	return emptyMap;
}

void EmptyMapGenerator::AddItem(Map* map, int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	map->actors.push(item);
}


