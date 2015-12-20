/*
 * MapGenerator.cpp
 *
 *  Created on: Nov 9, 2015
 */
#include "../../LoggerWrapper.hpp"
#include "../Map.hpp"
#include "MapGenerator.hpp"
#include "../../Tile/TileColors.hpp"


void MapGenerator::DrawHorizontalLine(Map* map, Point start, Point end, int character, TCODColor color) {
	try {
		for(u_int i = start.getX(); i <= end.getX() && i < map->GetWidth(); i++){
			Point temp = Point(i, end.getY());


			if(map->GetCharacter(temp.getX(), temp.getY() + 1) == TileCharacters::Default::DOUBLE_PIPE_VERTICAL){
				character = TileCharacters::Default::DOUBLE_PIPE_T_TOP;
			}
			else if(temp.getY() != 0 && map->GetCharacter(temp.getX(), temp.getY() - 1) == TileCharacters::Default::DOUBLE_PIPE_VERTICAL){
				character = TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM;
			}
			else{
				character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
			}
			map->SetTileProperties(temp, color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawHorizontalLine");
		throw 0;
	}
}


void MapGenerator::DrawVerticalLine(Map* map, Point start, Point end, int character, TCODColor color) {
	try {

		for(u_int i = start.getY(); i <= end.getY() && i < map->GetHeight(); i++){
			Point temp = Point(start.getX(), i);

			if(map->GetCharacter(temp.getX() + 1, temp.getY()) == TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL){
				character = TileCharacters::Default::DOUBLE_PIPE_T_LEFT;
			}
			else if(temp.getX() != 0 && map->GetCharacter(temp.getX() - 1, temp.getY()) == TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL){
				character = TileCharacters::Default::DOUBLE_PIPE_T_RIGHT;
			}
			else{
				character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			}
			map->SetTileProperties(temp, color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawVerticalLine");
		throw 0;
	}
}


void MapGenerator::DrawGrass(Map* map, TCODMap* roadMap, int x, int y){
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

void MapGenerator::DrawTree(Map* map, TCODMap* forestMap, int x, int y){
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


void MapGenerator::DrawRoad(Map* map, TCODMap* roadMap, int x, int y){
	try {
		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::grey;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawRoad");
		throw 0;
	}
}

void MapGenerator::DrawSidewalk(Map* map, TCODMap* roadMap, int x, int y){
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
