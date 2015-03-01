/*
 * RoadMapGenerator.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */



#include "../main.hpp"

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	int width = map->width;
	int height = map->height;
	TCODMap* emptyMap = new TCODMap(width, height);
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {

			if(tilex <= (width/2)+3  &&  tilex >= (width/2)-3){
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::lightGrey;
				map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::darkGrey;
				map->tiles[tilex+tiley*(map->width)].character = 46;
			}
			else{
				emptyMap->setProperties(tilex, tiley, true, true);
				map->tiles[tilex+tiley*(map->width)].visibleColor = TCODColor::darkGreen;
				map->tiles[tilex+tiley*(map->width)].fogColor = TCODColor::darkestGreen;
				map->tiles[tilex+tiley*(map->width)].character = 46;
			}
		}
	}
	return emptyMap;
}
