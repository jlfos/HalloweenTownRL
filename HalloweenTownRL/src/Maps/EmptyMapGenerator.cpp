/*
 * EmptyMapGenerator.cpp
 *
 *  Created on: Feb 14, 2015
 *
 */
#include <iostream>
#include "libtcod.hpp"
#include "EmptyMapGenerator.hpp"
#include "Map.hpp"

EmptyMapGenerator::EmptyMapGenerator(){
	seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
}

EmptyMapGenerator::EmptyMapGenerator(int seed) :seed(seed){
}

TCODMap* EmptyMapGenerator::Generate(Map* map, bool generateActors){
	int width = map->GetWidth();
	int height = map->GetHeight();
	TCODMap* emptyMap = new TCODMap(width, height);
	TCODRandom rng(seed, TCOD_RNG_MT);

	int potionCount = 0;
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {
			emptyMap->setProperties(tilex, tiley, true, true);
			int rand = rng.getInt(0, 400);
			int tileIndex = tilex+tiley*width;
			map->SetTileProperties(tileIndex, TCODColor::lighterRed, TCODColor::darkRed, Actor::CharacterCodes::PERIOD);
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


