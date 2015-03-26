/*
 * EmptyMapGenerator.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: josh
 */
#include <iostream>
#include "../main.hpp"

using namespace std;

EmptyMapGenerator::EmptyMapGenerator(){
	seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
}

EmptyMapGenerator::EmptyMapGenerator(int seed) :seed(seed){
}

TCODMap* EmptyMapGenerator::Generate(Map* map, bool generateActors){
	TCODMap* emptyMap = new TCODMap(map->width, map->height);
	TCODRandom rng(seed, TCOD_RNG_MT);

	int potionCount = 0;
	for (int tilex = 0; tilex < map->width; tilex++) {
		for (int tiley = 0; tiley < map->height; tiley++) {
			emptyMap->setProperties(tilex, tiley, true, true);
			int rand = rng.getInt(0, 400);
			map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::lighterRed;
			map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::darkRed;
			map->tiles[tilex+tiley*(map->width)].character = 46;
			if(rand%400==0 && generateActors ){
				AddItem(map, tilex, tiley);
				potionCount++;
			}
		}
	}
	return emptyMap;
}

void EmptyMapGenerator::PopulateActors(Map* map){

}

void EmptyMapGenerator::AddItem(Map* map, int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	map->actors.push(item);
}


