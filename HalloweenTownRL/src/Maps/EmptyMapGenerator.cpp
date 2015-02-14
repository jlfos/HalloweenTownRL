/*
 * EmptyMapGenerator.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: josh
 */
#include "../main.hpp"


TCODMap* EmptyMapGenerator::Generate(int width, int height){
	TCODMap* emptyMap = new TCODMap(width, height);
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {
			emptyMap->setProperties(tilex, tiley, true, true);
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = rng->getInt(0, 200);
			if(rand%100==0)
				AddItem(tilex, tiley);
		}
	}
	return emptyMap;
}

void EmptyMapGenerator::AddItem(int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	engine.actors.push(item);
}


