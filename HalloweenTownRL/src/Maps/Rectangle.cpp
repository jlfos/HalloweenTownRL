/*
 * Square.cpp
 *
 *  Created on: Dec 19, 2015
 */

#include "Rectangle.hpp"

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

void Rectangle::Draw(Map* map) {
	DrawNorthSide(map);
	DrawEastSide(map);
	DrawSouthSide(map);
	DrawWestSide(map);
}

void Rectangle::DrawNorthSide(Map* map) {
	map->SetTileProperties(nwCorner, color, details.nwCorner);
	for(int i = nwCorner.getX() + 1; i < seCorner.getX(); i++){
		map->SetTileProperties(Point(i, nwCorner.getY()), color, details.northSide);
	}
	map->SetTileProperties(Point(seCorner.getX(), nwCorner.getY()) , color, details.neCorner);
}

void Rectangle::DrawEastSide(Map* map) {
	map->SetTileProperties(Point(seCorner.getX(), nwCorner.getY()), color, details.neCorner);
	for(int i = nwCorner.getY() + 1; i < seCorner.getY(); i++){
		map->SetTileProperties(Point( seCorner.getX(), i), color, details.eastSide);
	}
	map->SetTileProperties(seCorner , color, details.seCorner);
}

void Rectangle::DrawSouthSide(Map* map) {
	map->SetTileProperties(Point(nwCorner.getX(), seCorner.getY()), color, details.swCorner);
	for(int i = nwCorner.getX() + 1; i < seCorner.getX(); i++){
		map->SetTileProperties(Point(i, seCorner.getY()), color, details.southSide);
	}
	map->SetTileProperties(seCorner, color, details.seCorner);
}

void Rectangle::DrawWestSide(Map* map) {
	map->SetTileProperties(nwCorner, color, details.nwCorner);
	for(int i = nwCorner.getY() + 1; i < seCorner.getY(); i++){
		map->SetTileProperties(Point( nwCorner.getX(), i), color, details.eastSide);
	}
	map->SetTileProperties(Point(nwCorner.getX(), seCorner.getY()) , color, details.swCorner);
}
