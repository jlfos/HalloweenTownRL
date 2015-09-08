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
	for (int tilex = 0; tilex < width-1; tilex++) {
		for (int tiley = 0; tiley < height-1; tiley++) {
			int tileIndex = tilex+tiley*width;
			if(!map->TileHasBeenSet(tileIndex)){
					forestMap->setProperties(tilex, tiley, true, true);
					GenerateGrass(tilex, tiley, width, forestMap, map);
					if(tilesTillNextSpawn==0){
						tilesTillNextSpawn =rng->getInt(5, 50);
						Point spawn(tilex, tiley);
						map->spawnLocations.push_back(spawn);
					}
					tilesTillNextSpawn--;
				}
		}
	}


	return forestMap;
}

void ForestMapGenerator::PopulateActors(Map* map){
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

/**
 * Generates specific tiles for the map
 */
void ForestMapGenerator::GenerateTile(int x, int y, int width, int height, TCODMap* forestMap, Map* map){
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
		GenerateTree(x, y, width, forestMap, map);

}

/**
 * Generates Tree tile
 */
void ForestMapGenerator::GenerateTree(int x, int y, int width, TCODMap* forestMap, Map* map){
	int tileIndex = x+y*width;
	forestMap->setProperties(x, y, false, false);
	TCODColor visible = TileColors::brown;
	int character = TileCharacters::Default::YEN_SYMBOL;
	map->SetTileProperties(tileIndex, visible, character);
}

/**
 *  Generates Grass tile
 */
void ForestMapGenerator::GenerateGrass(int x, int y, int width, TCODMap* forestMap, Map* map){
	int tileIndex = x+y*width;
	forestMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::green;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, character);
}

