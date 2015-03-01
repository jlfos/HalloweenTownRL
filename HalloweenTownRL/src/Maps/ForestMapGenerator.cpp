/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

TCODMap* ForestMapGenerator::Generate(Map* map, bool generateActors){
	TCODMap* emptyMap = new TCODMap(map->width, map->height);
	int width = map->width;
	int height = map->height;
	for (int tilex = 0; tilex < map->width; tilex++) {
		for (int tiley = 0; tiley < map->height; tiley++) {
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = 0;
			if(tilex<=width/2)
				rand = rng->getInt(25, 100);
			else
				rand = rng->getInt(78, 100);

			if(rand%100==0){
				emptyMap->setProperties(tilex, tiley, true, false);
				map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor(139, 69, 19);
				map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor(97, 49, 12);
				map->tiles[tilex+tiley*(map->width)].character = 190;
			}
			else{
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::green;
				map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::darkerGreen;
				map->tiles[tilex+tiley*(map->width)].character = 46;
			}
		}
	}
	return emptyMap;
}

void ForestMapGenerator::AddItem(Map* map, int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	map->actors.push(item);
}
