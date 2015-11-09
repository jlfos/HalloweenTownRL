/*
 * MapGenerator.cpp
 *
 *  Created on: Nov 9, 2015
 */
#include "../LoggerWrapper.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"



void MapGenerator::DrawHorizontalLine(Map* map, Point start, Point end, int character, TCODColor color) {
	try {
		for(u_int i = start.getX(); i <= end.getX() && i < map->GetWidth(); i++){
			map->SetTileProperties(Point(i, end.getY()), color, character);
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
			map->SetTileProperties(Point(start.getX(), i), color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawVerticalLine");
		throw 0;
	}
}
