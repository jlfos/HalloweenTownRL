/*
 * ForestMapGenerator.cpp
 *
 *  Created on: Feb 15, 2015
 */
#include <iostream>
#include "libtcod.hpp"
#include "../Actor/Actor.hpp"
#include "../Actor/ActorFactory.hpp"
#include "ForestMapGenerator.hpp"
#include "../LoggerWrapper.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"
#include "../Tile/TileColors.hpp"


ForestMapGenerator::ForestMapGenerator() : ForestMapGenerator(MapGenerator::Orientation::NORTH){

}

ForestMapGenerator::ForestMapGenerator(MapGenerator::Orientation orientation){
	rng = TCODRandom::getInstance();
	this->orientation = orientation;
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
	try {
		int width = map->GetWidth();
		int height = map->GetHeight();
		TCODMap* forestMap = new TCODMap(width, height);
		int tilesTillNextSpawn = 0;
		for (int tileX = 0; tileX < width; tileX++) {
				for (int tileY = 0; tileY < height; tileY++) {
						GenerateTile(tileX, tileY, width, height, forestMap, map);
				}
		}

		//TODO Make the generation of spawn locations more generic
		for (int x = 0; x < width-1; x++) {
			for (int y = 0; y < height-1; y++) {
				if(!map->TileHasBeenSet(x, y)){
						forestMap->setProperties(x, y, true, true);
						GenerateGrass(x, y, forestMap, map);
						if(tilesTillNextSpawn==0){
							tilesTillNextSpawn =rng->getInt(5, 50);
							Point spawn(x, y);
							map->spawnLocations.push_back(spawn);
						}
						tilesTillNextSpawn--;
					}
			}
		}
		return forestMap;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ForestMapGenerator::Generate");
		throw 0;
	}
}

void ForestMapGenerator::PopulateActors(Map* map){
	try {
		//TODO Work on making PopulateActors a little more generic
		map->actors.clear();

		int items = rng->getInt(1,3);
		int nextItem = 0;
		int nextSpawn = rng->getInt(5, 15);
		ActorFactory::EnemyDifficulty difficulty = map->GetDifficulty();
		for(Point spawn : map->spawnLocations){
			nextSpawn--;
			nextItem--;
			if(nextSpawn==0){
				Actor *temp;
				if(items>0 && nextItem <= 0){
					nextItem = rng->getInt(0, 10);
					items--;
					temp = ActorFactory::CreateItem(spawn.getX(), spawn.getY(), difficulty);
				}
				else{
					temp = ActorFactory::CreateMonster(spawn.getX(),
						 spawn.getY(),
						 difficulty,
						 ActorFactory::MapType::WOODS);
					}
				map->actors.push(temp);
				nextSpawn = rng->getInt(5, 10);
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ForestMapGenerator::PopulateActors");
		throw 0;
	}
}

/**
 * Generates specific tiles for the map
 */
void ForestMapGenerator::GenerateTile(int x, int y, int width, int height, TCODMap* forestMap, Map* map){
	try {
		bool lowTreeChance = true;
		switch(orientation){
			case MapGenerator::Orientation::EAST:
				lowTreeChance = x<=width/2;
				break;
			case MapGenerator::Orientation::WEST:
				lowTreeChance = x>=width/2;
				break;
			case MapGenerator::Orientation::NORTH:
				lowTreeChance = y>=height/2;
				break;
			case MapGenerator::Orientation::SOUTH:
				lowTreeChance = y<=height/2;
				break;
		}

		int rand = 0;
		if(lowTreeChance)
			rand = rng->getInt(80, 100);
		else
			rand = rng->getInt(90, 100);
		if(rand%100==0 && x!=0 && y!=0 && x!=width-1 && y!=height-1)
			GenerateTree(x, y, forestMap, map);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ForestMapGenerator::GenerateTile");
		throw 0;
	}

}

/**
 * Generates Tree tile
 */
void ForestMapGenerator::GenerateTree(int x, int y, TCODMap* forestMap, Map* map){
	try {
		forestMap->setProperties(x, y, false, false);
		TCODColor visible = TileColors::brown;
		int character = TileCharacters::Default::YEN_SYMBOL;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ForestMapGenerator::GenerateTree");
		throw 0;
	}
}

/**
 *  Generates Grass tile
 */
void ForestMapGenerator::GenerateGrass(int x, int y, TCODMap* forestMap, Map* map){
	try {
		forestMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::green;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ForestMapGenerator::GenerateGrass");
		throw 0;
	}
}

