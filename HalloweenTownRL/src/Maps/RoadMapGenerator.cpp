/*
 * RoadMapGenerator.cpp
 *
 *  Created on: Feb 28, 2015
 */
#include <iostream>
#include <memory>
#include <set>
#include "libtcod.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"
#include "RoadMapGenerator.hpp"
#include "../Tile/TileColors.hpp"
#include "../LoggerWrapper.hpp"

#ifndef RMG_LOGGER
//#define RMG_LOGGER
#endif

RoadMapGenerator::RoadMapGenerator():RoadMapGenerator(MapGenerator::Orientation::NORTH) {

}
RoadMapGenerator::RoadMapGenerator(MapGenerator::Orientation orientation){
	this->flagOri = orientation;
	map = nullptr;
	roadMap = nullptr;
	minSizeX = 5;
	minSizeY = 5;
	maxSizeX = 9;
	maxSizeY = 9;
}




TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	try{
		int width = map->GetWidth();
		int height = map->GetHeight();
		roadMap = new TCODMap(width, height);
		this->map = map;
		TCODColor visible = TCODColor::grey;

		//TODO This is as close to 0,0 as I can go. I get exceptions otherwise. I need to look into this
		int lotX = 10;
		int lotY = 10;


		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height-1; y++) {
				//TODO this needs to be set properly for the demo and lightsource needs to be fixed

				if(map->TileHasBeenSet(x, y)){
					if(map->GetCharacter(x, y) != TileCharacters::PERIOD){
						roadMap->setProperties(x, y, false, false);
						continue;
					}
					else{
						roadMap->setProperties(x, y, true, true);
					}
				}
				else{
					roadMap->setProperties(x, y, true, true);

					bool roadFlag = false;
					switch(flagOri){
						case MapGenerator::Orientation::NORTH:
						case MapGenerator::Orientation::SOUTH:
							roadFlag = x <= (width/2)+3  &&  x >= (width/2)-3;
							break;
						case MapGenerator::Orientation::EAST:
						case MapGenerator::Orientation::WEST:
							roadFlag = y <= (height/2)+3  &&  y >= (height/2)-3;
							break;
						default:
							LoggerWrapper::Error("Case " + std::to_string(flagOri) + " is not currently supported");
							break;
					}

					if(roadFlag)
						DrawRoad(x, y, roadMap);
					else
					DrawGrass(x, y, roadMap);
				}

			}
		}
		map->actors.push(ActorFactory::CreateLampPost(65, 7));

		return roadMap;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::Generate");
		throw 0;
	}
}


void RoadMapGenerator::PopulateActors(Map* map){

}

void RoadMapGenerator::DrawGrass(int x, int y, TCODMap* roadMap){
	try {
		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::green;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawGrass");
		throw 0;
	}
}


void RoadMapGenerator::DrawRoad(int x, int y, TCODMap* roadMap){
	try {
		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::lightGrey;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawRoad");
		throw 0;
	}
}
