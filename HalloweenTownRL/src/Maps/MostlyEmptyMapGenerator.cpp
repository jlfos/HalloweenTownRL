/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

TCODMap* MostlyEmptyMapGenerator::Generate(Map* map, bool generateActors){
	TCODMap* emptyMap = new TCODMap(map->width, map->height);
	for (int tilex = 0; tilex < map->width; tilex++) {
		for (int tiley = 0; tiley < map->height; tiley++) {
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = rng->getInt(0, 200);
			if(rand%200==0){
				emptyMap->setProperties(tilex, tiley, true, false);
				map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::lighterRed;
				map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::darkRed;
				map->tiles[tilex+tiley*(map->width)].character = 35;
			}
			else{
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::lighterRed;
				map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::darkRed;
				map->tiles[tilex+tiley*(map->width)].character = 46;
			}
				if(rand%100==0 && generateActors)
					AddItem(map, tilex, tiley);
		}
	}
	return emptyMap;
}

void MostlyEmptyMapGenerator::AddItem(Map* map, int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	map->actors.push(item);
}
