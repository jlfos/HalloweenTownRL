/*
 * CityMapGenerator.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: josh
 */


#include "../main.hpp"

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
					map->tiles[tilex+tiley*(map->width)].character = 46;
				}
		}
	}




	return cityMap;
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
			cityMap->setProperties(tilex, tiley, false, false);
			map->tiles[tilex+tiley*(map->width)].visibleColor = visible;
			map->tiles[tilex+tiley*(map->width)].fogColor = fog;
			if(tilex==startX && tiley==startY)
				map->tiles[tilex+tiley*(map->width)].character = 201;
			else if(tilex==startX && tiley == startY+sizeY-1)
				map->tiles[tilex+tiley*(map->width)].character = 200;
			else if(tilex==startX+sizeX-1 && tiley==startY)
				map->tiles[tilex+tiley*(map->width)].character = 187;
			else if(tilex==startX+sizeX-1 && tiley==startY+sizeY-1)
				map->tiles[tilex+tiley*(map->width)].character = 188;
			else if(tilex==startX || tilex==startX+sizeX-1)
				map->tiles[tilex+tiley*(map->width)].character = 186;
			else if(tiley==startY || tiley == startY+sizeY-1)
				map->tiles[tilex+tiley*(map->width)].character = 205;
			else
				map->tiles[tilex+tiley*(map->width)].character = 35;
		}
	}
}


