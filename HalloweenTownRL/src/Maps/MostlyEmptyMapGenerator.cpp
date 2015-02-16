/*
 * MostlyEmptyMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: josh
 */

#include "../main.hpp"

TCODMap* MostlyEmptyMapGenerator::Generate(int width, int height){
	TCODMap* emptyMap = new TCODMap(width, height);
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {
			TCODRandom *rng = TCODRandom::getInstance();
			int rand = rng->getInt(0, 200);
			if(rand%200==0)
				emptyMap->setProperties(tilex, tiley, true, false);
			else
				emptyMap->setProperties(tilex, tiley, true, true);


			if(rand%100==0)
				AddItem(tilex, tiley);
		}
	}
	return emptyMap;
}

void MostlyEmptyMapGenerator::AddItem(int x, int y){
	Actor* item = ActorFactory::CreatePotion(x, y);
	engine.actors.push(item);
}
