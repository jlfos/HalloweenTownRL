/*
 * RoadMapGenerator.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */



#include "../main.hpp"

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	int width = map->GetWidth();
	int height = map->GetHeight();
	int enemies = 0;
	TCODMap* emptyMap = new TCODMap(width, height);
	TCODColor visible;
	TCODColor fog;
	int characterCode;
	for (int tilex = 0; tilex < width; tilex++) {
		for (int tiley = 0; tiley < height; tiley++) {
			int tileIndex = tilex+tiley*width;

			if(tilex <= (width/2)+3  &&  tilex >= (width/2)-3){
				visible =  TCODColor::lightGrey;
				fog = TCODColor::darkGrey;
			}
			else{
				visible = TCODColor::darkGreen;
				fog = TCODColor::darkestGreen;
			}
			emptyMap->setProperties(tilex, tiley, true, true);
			map->SetTileProperties(tileIndex, visible, fog, Actor::CharacterCodes::PERIOD);
		}
	}

	return emptyMap;
}


void RoadMapGenerator::PopulateActors(Map* map){

}
