/*
 * CityMapGenerator.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: josh
 */
#include <iostream>
#include "../main.hpp"


CityMapGenerator::CityMapGenerator(){
	rng = TCODRandom::getInstance();

	eastWestStreet = rng->getInt(2,4);
	northSouthStreet = rng->getInt(2,4);
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

	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {
			int tileIndex = tilex+tiley*width;
			if(!map->TileHasBeenSet(tileIndex)){
					cityMap->setProperties(tilex, tiley, true, true);
					map->SetTileProperties(tileIndex, TCODColor::lighterGrey, TCODColor::grey, Actor::CharacterCodes::PERIOD);
					if(tilesTillNextSpawn==0){
						tilesTillNextSpawn =rng->getInt(5, 50);
						Point spawn;
						spawn.x = tilex;
						spawn.y = tiley;
						map->spawnLocations.push_back(spawn);
					}
					tilesTillNextSpawn--;
				}
		}
	}

	return cityMap;
}
/**
 * Populates the actors on a map based off the current difficulty
 */
void CityMapGenerator::PopulateActors(Map* map){
	try{
		map->actors.clear();
		int nextSpawn = rng->getInt(5, 15);
		ActorFactory::EnemyDifficulty difficulty = map->GetDifficulty();
		for(Point spawn : map->spawnLocations){
			nextSpawn--;
			if(nextSpawn==0){
				map->actors.push(ActorFactory::CreateMonster(spawn.x,
															 spawn.y,
															 difficulty,
															 ActorFactory::MapType::CITY));
				nextSpawn = rng->getInt(5, 10);
			}
		}
	}
	catch(...){
		std::cerr << "An error occurred in CityMapGenerator::PopulateActors()" << std::endl;
	}
}

/*
 * Generates a building of random color and size.
 */
void CityMapGenerator::CreateBuilding(Map* map, TCODMap* cityMap, int startX, int startY){
	TCODColor visible;
	TCODColor fog;
	int sizeX = rng->getInt(3, 8);
	int sizeY = rng->getInt(3, 8);
	int width = map->GetWidth();
	GenerateBuildingColors(visible, fog);
	int height = map->GetHeight();
	for(int tileX = startX; tileX < startX+sizeX && tileX < width-1; tileX++ ){
		for(int tileY = startY; tileY < startY+sizeY && tileY < height-1; tileY++){

			int tileIndex =  tileX + tileY* width;
			int character = GenerateBuildingCharacter(startX, startY, tileX, tileY, sizeX, sizeY);

			cityMap->setProperties(tileX, tileY, false, false);
			map->SetTileProperties(tileIndex, visible, fog, character);
		}
	}
}
/**
 * Generates building colors (visible and fog of war). Based off of rng.
 * Current choices are crimson, sepia and grey
 */
void CityMapGenerator::GenerateBuildingColors(TCODColor& visible, TCODColor& fog){
	int color = rng->getInt(1, 3);
	if(color == 1){
		visible = TCODColor::darkerGrey;
		fog = TCODColor::darkestGrey;
	}
	else if(color == 2){
		visible = TCODColor::desaturatedCrimson;
		fog = TCODColor::darkestCrimson;
	}
	else{
		visible = TCODColor::darkerSepia;
		fog = TCODColor::darkestSepia;
	}

}

/*
 * Generates a character code for a building (sides, corners, etc) based off the start coordinates,
 * the end coordinates, and the size of the building
 */
int CityMapGenerator::GenerateBuildingCharacter(int startX, int startY, int currentX, int currentY, int sizeX, int sizeY){
	int characterCode = Actor::CharacterCodes::RAINBOW;
	if(currentX == startX && currentY == startY)
		characterCode = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_UPPER_LEFT;
	else if(currentX == startX && currentY == startY+sizeY-1)
		characterCode = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_LOWER_LEFT;
	else if(currentX == startX+sizeX-1 && currentY == startY)
		characterCode = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
	else if(currentX == startX+sizeX-1 && currentY == startY+sizeY-1)
		characterCode = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
	else if(currentX == startX || currentX == startX+sizeX-1)
		characterCode = Actor::CharacterCodes::DOUBLE_PIPE_VERTICAL;
	else if(currentY == startY || currentY == startY+sizeY-1)
		characterCode = Actor::CharacterCodes::DOUBLE_PIPE_HORIZONTAL;
	else	//Interior of the building
		characterCode = Actor::CharacterCodes::BLOCK_FULL;
	return characterCode;
}
