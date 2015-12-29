/*
 * Square.cpp
 *
 *  Created on: Dec 19, 2015
 */

#include "../LoggerWrapper.hpp"
#include "Map.hpp"
#include "Rectangle.hpp"
#include "../Tile/TileCharacters.hpp"


Rectangle::Rectangle(Point nwCorner, Point seCorner, TCODColor color) :
Rectangle(nwCorner, seCorner, color, TileCharacters::Default::RAINBOW){
}


Rectangle::Rectangle(Point nwCorner, Point seCorner, TCODColor color, int character) :
	nwCorner(nwCorner), seCorner(seCorner), color(color){

	this->details.neCorner = character;
	this->details.northSide = character;
	this->details.nwCorner = character;
	this->details.eastSide = character;
	this->details.swCorner = character;
	this->details.southSide = character;
	this->details.seCorner = character;
	this->details.eastSide = character;
}

Rectangle::Rectangle(Point nwCorner, Point seCorner, TCODColor color, RectangleDetails details) :
		nwCorner(nwCorner), seCorner(seCorner), color(color), details(details) {
}

void Rectangle::Draw(Map* map, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		if(nwCorner.getX() > seCorner.getX() || nwCorner.getY() > seCorner.getY()){
			LoggerWrapper::Error("Start X(" + std::to_string(nwCorner.getX()) + "), Y("+std::to_string(nwCorner.getY())+
					")must be less than End X(" + std::to_string(seCorner.getX()) + "), Y("+std::to_string(seCorner.getY())+")");
			throw 0;
		}

		DrawNorthSide(map, skipFilledTiles);
		DrawEastSide(map, skipFilledTiles);
		DrawSouthSide(map, skipFilledTiles);
		DrawWestSide(map, skipFilledTiles);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Rectangle::Draw");
		throw 0;
	}
}

void Rectangle::DrawNorthSide(Map* map, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		map->SetTileProperties(nwCorner, color, details.nwCorner);
		for(int i = nwCorner.getX() + 1; i < seCorner.getX(); i++){
			Point temp = Point(i, nwCorner.getY());
			if(skipFilledTiles && map->TileHasBeenSet(temp)){
					continue;
			}
			map->SetTileProperties(temp, color, details.northSide);
		}
		map->SetTileProperties(Point(seCorner.getX(), nwCorner.getY()) , color, details.neCorner);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Rectangle::DrawNorthSide");
		throw 0;
	}
}

void Rectangle::DrawEastSide(Map* map, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		map->SetTileProperties(Point(seCorner.getX(), nwCorner.getY()), color, details.neCorner);
		for(int i = nwCorner.getY() + 1; i < seCorner.getY(); i++){
			Point temp = Point(seCorner.getX(), i);
			if(skipFilledTiles && map->TileHasBeenSet(temp)){
					continue;
			}
			map->SetTileProperties(temp, color, details.eastSide);
		}
		map->SetTileProperties(seCorner , color, details.seCorner);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Rectangle::DrawEastSide");
		throw 0;
	}
}

void Rectangle::DrawSouthSide(Map* map, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		map->SetTileProperties(Point(nwCorner.getX(), seCorner.getY()), color, details.swCorner);
		for(int i = nwCorner.getX() + 1; i < seCorner.getX(); i++){
			Point temp = Point(i, seCorner.getY());
			if(skipFilledTiles && map->TileHasBeenSet(temp)){
					continue;
			}
			map->SetTileProperties(temp, color, details.southSide);
		}
		map->SetTileProperties(seCorner, color, details.seCorner);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Rectangle::DrawSouthSide");
		throw 0;
	}
}

void Rectangle::DrawWestSide(Map* map, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		map->SetTileProperties(nwCorner, color, details.nwCorner);
		for(int i = nwCorner.getY() + 1; i < seCorner.getY(); i++){
			Point temp = Point(nwCorner.getX(), i);
			if(skipFilledTiles && map->TileHasBeenSet(temp)){
					continue;
			}
			map->SetTileProperties(temp, color, details.eastSide);
		}
		map->SetTileProperties(Point(nwCorner.getX(), seCorner.getY()) , color, details.swCorner);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Rectangle::DrawWestSide");
		throw 0;
	}
}

Rectangle::~Rectangle() {
}
