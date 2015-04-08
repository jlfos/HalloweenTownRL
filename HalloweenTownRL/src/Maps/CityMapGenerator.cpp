/*
 * CityMapGenerator.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: josh
 */
#include <iostream>
#include "../main.hpp"

using namespace std;

CityMapGenerator::CityMapGenerator(){
	TCODRandom *rng = TCODRandom::getInstance();

	eastWestStreet = rng->getInt(2,4);
	northSouthStreet = rng->getInt(2,4);
}

TCODMap* CityMapGenerator::Generate(Map* map, bool generateActors){
	TCODMap* cityMap = new TCODMap(map->width, map->height);
	int width = map->width;
	int height = map->height;
	TCODRandom *rng = TCODRandom::getInstance();
	int buildingSize = 5;
	int tilesTillNextSpawn = 0;
	for(int j =1;(eastWestStreet*j)+(buildingSize*(j-1))<height;j++){
		for(int i = 1;(northSouthStreet*i)+(buildingSize*(i-1))<width; i++){
			CreateBuilding(map, cityMap, 0+(eastWestStreet*i)+(buildingSize*(i-1)),
										 0+(northSouthStreet*j)+(buildingSize*(j-1)));
		}
	}

	for (int tilex = 0; tilex < map->width; tilex++) {
		for (int tiley = 0; tiley < map->height; tiley++) {
				if(map->tiles[tilex+tiley*(map->width)].character==0){
					cityMap->setProperties(tilex, tiley, true, true);
					map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::lighterGrey;
					map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::grey;
					map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::PERIOD;
					if(tilesTillNextSpawn==0){
						tilesTillNextSpawn =rng->getInt(5, 50);
						Point spawn;
						spawn.x = tilex;
						spawn.y = tiley;
						map->spawnLocations.push_back(spawn);
						//map->actors.push(ActorFactory::CreateImp(tilex, tiley));
					}
					tilesTillNextSpawn--;
				}
		}
	}

	return cityMap;
}

void CityMapGenerator::PopulateActors(Map* map){
	try{
		TCODRandom *rng = TCODRandom::getInstance();
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
		cerr << "An error occurred in CityMapGenerator::PopulateActors()" << endl;
	}
}


void CityMapGenerator::CreateBuilding(Map* map, TCODMap* cityMap, int startX, int startY){
	TCODRandom *rng = TCODRandom::getInstance();
	int sizeX = rng->getInt(3, 8);
	int sizeY = rng->getInt(3, 8);
	int color = rng->getInt(1, 3);
	TCODColor visible;
	TCODColor fog;
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
	for(int tilex = startX; tilex < startX+sizeX && tilex < map->width-1; tilex++ ){
		for(int tiley = startY; tiley < startY+sizeY && tiley < map->height-1; tiley++){
			cityMap->setProperties(tilex, tiley, true, false);
			map->tiles[tilex+tiley*(map->width)].visibleColor = visible;
			map->tiles[tilex+tiley*(map->width)].fogColor = fog;
			if(tilex==startX && tiley==startY)
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_UPPER_LEFT;
			else if(tilex==startX && tiley == startY+sizeY-1)
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_LOWER_LEFT;
			else if(tilex==startX+sizeX-1 && tiley==startY)
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
			else if(tilex==startX+sizeX-1 && tiley==startY+sizeY-1)
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
			else if(tilex==startX || tilex==startX+sizeX-1)
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::DOUBLE_PIPE_VERTICAL;
			else if(tiley==startY || tiley == startY+sizeY-1)
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::DOUBLE_PIPE_HORIZONTAL;
			else
				map->tiles[tilex+tiley*(map->width)].character = Actor::CharacterCodes::BLOCK_FULL;
		}
	}
}


