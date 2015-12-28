/*
 * MapGenerator.cpp
 *
 *  Created on: Nov 9, 2015
 */
#include "../../LoggerWrapper.hpp"
#include "../Map.hpp"
#include "MapGenerator.hpp"
#include "../../Tile/TileColors.hpp"
#include "../Rectangle.hpp"


void MapGenerator::DrawHorizontalLine(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}
		if(start.getX() > end.getX()){
			LoggerWrapper::Error("Start X(" + std::to_string(start.getX()) + "), Y("+std::to_string(start.getY())+
					")must be less than End X(" + std::to_string(end.getX()) + "), Y("+std::to_string(end.getY())+")");
			throw 0;
		}

		for(u_int i = start.getX(); i <= end.getX() && i < map->GetWidth(); i++){
			Point temp = Point(i, end.getY());
			if(skipFilledTiles && map->TileHasBeenSet(temp)){
					continue;
			}
			map->SetTileProperties(temp, color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawHorizontalLine");
		throw 0;
	}
}


void MapGenerator::DrawHorizontalLine(Map* map, Point start, Point end, TCODColor color, int character) {
	try {
		DrawHorizontalLine(map, start, end, color, character, false);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawHorizontalLine");
		throw 0;
	}
}


void MapGenerator::DrawVerticalLine(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}
		if(start.getY() > end.getY()){
			LoggerWrapper::Error("Start X(" + std::to_string(start.getX()) + "), Y("+std::to_string(start.getY())+
					")must be less than End X(" + std::to_string(end.getX()) + "), Y("+std::to_string(end.getY())+")");
			throw 0;
		}

		for(u_int i = start.getY(); i <= end.getY() && i < map->GetHeight(); i++){
			Point temp = Point(start.getX(), i);
			if(skipFilledTiles && map->TileHasBeenSet(temp)){
					continue;
			}
			map->SetTileProperties(temp, color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawVerticalLine");
		throw 0;
	}
}

void MapGenerator::DrawVerticalLine(Map* map, Point start, Point end, TCODColor color, int character) {
	try {
		DrawVerticalLine(map, start, end, color, character, false);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawVerticalLine");
		throw 0;
	}
}



void MapGenerator::DrawFilledArea(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}
		if(start.getX() > end.getX() || start.getY() > end.getY()){
			LoggerWrapper::Error("Start X(" + std::to_string(start.getX()) + "), Y("+std::to_string(start.getY())+
					")must be less than End X(" + std::to_string(end.getX()) + "), Y("+std::to_string(end.getY())+")");
			throw 0;
		}


		for(u_int i = start.getX(); i <= end.getX() && i < map->GetWidth(); i++){
			for(u_int j = start.getY(); j <= end.getY() && j < map->GetHeight(); j++){
				Point temp = Point(i, j);
				if(skipFilledTiles && map->TileHasBeenSet(temp)){
						continue;
				}
				map->SetTileProperties(temp, color, character);
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawHorizontalLine");
		throw 0;
	}
}

void MapGenerator::DrawFilledArea(Map* map, Point start, Point end, TCODColor color, int character) {
	try {
		DrawFilledArea(map, start, end, color, character, false);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawHorizontalLine");
		throw 0;
	}
}

void MapGenerator::DrawRectangle(Map* map, Point start, Point end, TCODColor color, int character, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}
		if(start.getX() > end.getX() || start.getY() > end.getY()){
			LoggerWrapper::Error("Start X(" + std::to_string(start.getX()) + "), Y("+std::to_string(start.getY())+
					")must be less than End X(" + std::to_string(end.getX()) + "), Y("+std::to_string(end.getY())+")");
			throw 0;
		}

		Rectangle rectangle(start, end, color, character);
		rectangle.Draw(map, skipFilledTiles);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawRectangle");
		throw 0;
	}
}

void MapGenerator::DrawRectangle(Map* map, Point start, Point end, TCODColor color, int character) {
	try {
		DrawRectangle(map, start, end, color, character, false);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawRectangle");
		throw 0;
	}
}

void MapGenerator::DrawGrass(Map* map, TCODMap* roadMap, int x, int y){
	try {
		if(map == nullptr || roadMap == nullptr){
			LoggerWrapper::Error("Pointers cannot be null");
			throw 0;
		}

		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::green;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawGrass");
		throw 0;
	}
}

void MapGenerator::DrawTree(Map* map, TCODMap* forestMap, int x, int y){
	try {
		if(map == nullptr || forestMap == nullptr){
			LoggerWrapper::Error("Pointers cannot be null");
			throw 0;
		}



		forestMap->setProperties(x, y, false, false);
		TCODColor visible = TileColors::brown;
		int character = TileCharacters::Default::YEN_SYMBOL;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawTree");
		throw 0;
	}
}


void MapGenerator::DrawRoad(Map* map, TCODMap* roadMap, int x, int y){
	try {
		if(map == nullptr || roadMap == nullptr){
			LoggerWrapper::Error("Pointers cannot be null");
			throw 0;
		}

		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::grey;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawRoad");
		throw 0;
	}
}

void MapGenerator::DrawSidewalk(Map* map, TCODMap* roadMap, int x, int y){
	try {
		if(map == nullptr || roadMap == nullptr){
			LoggerWrapper::Error("Pointers cannot be null");
			throw 0;
		}


		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::greyLighter;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawRoad");
		throw 0;
	}
}

void MapGenerator::DrawHorizontalWindow(Map* map, TCODMap* roadMap, int x, int y) {
	try {
		if(map == nullptr || roadMap == nullptr){
			LoggerWrapper::Error("Pointers cannot be null");
			throw 0;
		}


		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::blueLightest;
		int character = TileCharacters::Default::WINDOW_HORIZONTAL;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawHorizontalWindow");
		throw 0;
	}
}


void MapGenerator::DrawVerticalWindow(Map* map, TCODMap* roadMap, int x, int y) {
	try {
		if(map == nullptr || roadMap == nullptr){
			LoggerWrapper::Error("Pointers cannot be null");
			throw 0;
		}


		roadMap->setProperties(x, y, true, true);
		TCODColor visible = TileColors::blueLightest;
		int character = TileCharacters::Default::WINDOW_VERTICAL;
		map->SetTileProperties(x, y, visible, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in MapGenerator::DrawVerticalWindow");
		throw 0;
	}
}
