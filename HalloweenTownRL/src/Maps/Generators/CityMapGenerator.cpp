/*
 * CityMapGenerator.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: josh
 */
#include <iostream>
#include "libtcod.hpp"
#include "CityMapGenerator.hpp"
#include "../../LoggerWrapper.hpp"
#include "../Map.hpp"
#include "../../Tile/TileCharacters.hpp"
#include "../../Tile/TileColors.hpp"

CityMapGenerator::CityMapGenerator(bool boss) :boss(boss) {
	rng = TCODRandom::getInstance();

	eastWestStreet = rng->getInt(0,3);
	northSouthStreet = rng->getInt(3,5);
}

CityMapGenerator::~CityMapGenerator(){
	if(rng != nullptr){
		delete rng;
		rng = nullptr;
	}
}

/**
 *  Generates the tiles for the map
 */
TCODMap* CityMapGenerator::Generate(Map* map, bool generateActors){
	try {
		int width = map->GetWidth();
		int height = map->GetHeight();

		TCODMap* cityMap = new TCODMap(width, height);
		int buildingSize = 5;
		int tilesTillNextSpawn = 0;
		for(int j =1;(eastWestStreet*j)+(buildingSize*(j-1))<height;j++){
			for(int i = 1;(northSouthStreet*i)+(buildingSize*(i-1))<width; i++){
				CreateBuilding(map, cityMap, 0+(eastWestStreet*i)+(buildingSize*(i-1)),
											 0+(northSouthStreet*j)+(buildingSize*(j-1)));
			}
		}

		for (int x = 0; x < width-1; x++) {
			for (int y = 0; y < height-1; y++) {
				if(!map->TileHasBeenSet(x, y)){
						cityMap->setProperties(x, y, true, true);
						map->SetTileProperties(x, y, TileColors::greyLighter, TileCharacters::Default::PERIOD);
						if(tilesTillNextSpawn==0){
							tilesTillNextSpawn =rng->getInt(5, 50);
							Point spawn(x, y);
							map->spawnLocations.push_back(spawn);
						}
						tilesTillNextSpawn--;
				}
			}
		}

		return cityMap;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in CityMapGenerator::Generate");
		throw 0;
	}
}
/**
 * Populates the actors on a map based off the current difficulty
 */
void CityMapGenerator::PopulateActors(Map* map){
	try{
		map->actors.clear();
		if(!boss){
			int items = rng->getInt(0,2);
			int nextItem = 0;
			int nextSpawn = rng->getInt(5, 15);
			ActorFactory::EnemyDifficulty difficulty = map->GetDifficulty();
			for(Point spawn : map->spawnLocations){
				nextSpawn--;
				if(nextSpawn==0){
					Actor *temp;
					if(items>0 && nextItem <= 0){
						nextItem = rng->getInt(0, 35);
						items--;
						temp = ActorFactory::CreateItem(spawn.getX(), spawn.getY(), difficulty);
					}
					else{
						temp = ActorFactory::CreateMonster(spawn.getX(),
								 spawn.getY(),
								 difficulty,
								 ActorFactory::MapType::CITY);
					}

					map->actors.push(temp);
					nextSpawn = rng->getInt(5, 10);
				}
			}
		}
		else{
			int bossSpawn = rng->getInt(0, map->spawnLocations.size()-1);
			map->actors.push(ActorFactory::CreateGiantSpider(map->spawnLocations[bossSpawn].getX(), map->spawnLocations[bossSpawn].getY()));
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in CityMapGenerator::PopulateActors()");
		throw 0;
	}
}

/*
 * Generates a building of random color and size.
 */
void CityMapGenerator::CreateBuilding(Map* map, TCODMap* cityMap, int startX, int startY){
	try {
		TCODColor visible;
		int sizeX = rng->getInt(3, 8);
		int sizeY = rng->getInt(3, 8);
		int width = map->GetWidth();
		GenerateBuildingColor(visible);
		int height = map->GetHeight();
		for(int tileX = startX; tileX < startX+sizeX && tileX < width-2; tileX++ ){
			for(int tileY = startY; tileY < startY+sizeY && tileY < height-2; tileY++){
				int character = GenerateBuildingCharacter(startX, startY, tileX, tileY, sizeX, sizeY);

				cityMap->setProperties(tileX, tileY, false, false);
				map->SetTileProperties(tileX, tileY, visible, character);
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in CityMapGenerator::CreateBuilding");
		throw 0;
	}
}
/**
 * Generates building colors (visible and fog of war). Based off of rng.
 * Current choices are crimson, sepia and grey
 */
void CityMapGenerator::GenerateBuildingColor(TCODColor& visible){
	try {
		int color = rng->getInt(1, 3);
		if(color == 1){
			visible = TileColors::greyDarker;
		}
		else if(color == 2){
			visible = TileColors::crimsonDesaturated;
		}
		else{
			visible = TileColors::sepiaDarker;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in CityMapGenerator::GenerateBuildingColor");
		throw 0;
	}

}

/*
 * Generates a character code for a building (sides, corners, etc) based off the start coordinates,
 * the end coordinates, and the size of the building
 */
int CityMapGenerator::GenerateBuildingCharacter(int startX, int startY, int currentX, int currentY, int sizeX, int sizeY){
	try {
		int characterCode = TileCharacters::Default::RAINBOW;
		if(currentX == startX && currentY == startY)
			characterCode = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
		else if(currentX == startX && currentY == startY+sizeY-1)
			characterCode = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
		else if(currentX == startX+sizeX-1 && currentY == startY)
			characterCode = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
		else if(currentX == startX+sizeX-1 && currentY == startY+sizeY-1)
			characterCode = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
		else if(currentX == startX || currentX == startX+sizeX-1)
			characterCode = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
		else if(currentY == startY || currentY == startY+sizeY-1)
			characterCode = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
		else	//Interior of the building
			characterCode = TileCharacters::Default::BLOCK_FULL;
		return characterCode;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in CityMapGenerator::GenerateBuildingCharacter");
		throw 0;
	}
}
