/*
 * Room.cpp
 *
 *  Created on: Sep 7, 2015
 */
#include <algorithm>
#include "../LoggerWrapper.hpp"
#include "Room.hpp"
#include "../Tile/TileColors.hpp"
#include "Rectangle.hpp"
Room::Room(Point nwCorner, Point seCorner, MapGenerator::Orientation orientation) :
Rectangle(nwCorner, seCorner, TileColors::white), nwCorner(nwCorner), seCorner(seCorner), orientation(orientation)
{
	details.nwCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
	details.northSide = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
	details.neCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
	details.eastSide = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
	details.seCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
	details.southSide = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
	details.swCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
	details.westSide = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
}

Room::Room(Point nwCorner, int offsetX, int offsetY, MapGenerator::Orientation orientation)
: Rectangle(nwCorner, seCorner, TileColors::white),  nwCorner(nwCorner), seCorner(nwCorner.getX() + offsetX, nwCorner.getY() + offsetY), orientation(orientation){

	details.nwCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
	details.northSide = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
	details.neCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
	details.eastSide = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
	details.seCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
	details.southSide = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
	details.swCorner = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
	details.westSide = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
}


MapGenerator::Orientation Room::getOrientation() const {
	return orientation;
}



const Point& Room::getNECorner() const {
	return Point(seCorner.getX(), nwCorner.getY());
}

const Point& Room::getNWCorner() const {
	return nwCorner;
}

const Point& Room::getSECorner() const {
	return seCorner;
}

const Point& Room::getSWCorner() const {
	return Point(nwCorner.getX(), seCorner.getY());
}

void Room::Draw(Map* map, bool skipFilledTiles) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		Point neCorner = Point(seCorner.getX(), nwCorner.getY());
		DrawHorizontalLine(map, nwCorner, neCorner);
		Point swCorner = Point(nwCorner.getX(), seCorner.getY());
		DrawHorizontalLine(map, swCorner, seCorner);
		DrawVerticalLine(map, neCorner, seCorner);
		DrawVerticalLine(map, nwCorner, swCorner);
		DrawNWCorner(map, nwCorner);
		DrawNECorner(map, neCorner);
		DrawSECorner(map, seCorner);
		DrawSWCorner(map, swCorner);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Room::Draw");
		throw 0;
	}
}


void Room::DrawHorizontalLine(Map* map, Point start, Point end) {
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


		int character = TileCharacters::Default::RAINBOW;
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
		LoggerWrapper::Error("An error occurred in Room::DrawHorizontalLine");
		throw 0;
	}
}

void Room::SetWindowsEast(bool windowsEast) {
	this->windowsEast = windowsEast;
}

void Room::SetWindowsNorth(bool windowsNorth) {
	this->windowsNorth = windowsNorth;
}

void Room::SetWindowsSouth(bool windowsSouth) {
	this->windowsSouth = windowsSouth;
}

void Room::SetWindowsWest(bool windowsWest) {
	this->windowsWest = windowsWest;
}

void Room::DrawVerticalLine(Map* map, Point start, Point end) {
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


		int character;
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
		LoggerWrapper::Error("An error occurred in Room::DrawVerticalLine");
		throw 0;
	}
}

void Room::DrawNECorner(Map* map, Point point) {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		int connectionsEast[] =  { TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT, TileCharacters::Default::DOUBLE_PIPE_CROSS, TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM, TileCharacters::Default::DOUBLE_PIPE_T_TOP};
		int connectionsNorth[] = { TileCharacters::Default::DOUBLE_PIPE_VERTICAL, TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT, TileCharacters::Default::DOUBLE_PIPE_T_LEFT };

		bool eastConnect = false;

		bool northConnect = false;

		if(point.getY() > 0){
			northConnect = std::find(std::begin(connectionsNorth), std::end(connectionsNorth), map->GetCharacter(point.getX(), point.getY() - 1)) != std::end(connectionsNorth);
		}
		eastConnect = std::find(std::begin(connectionsEast), std::end(connectionsEast), map->GetCharacter(point.getX() + 1, point.getY())) != std::end(connectionsEast);
		int character;
		if(eastConnect || northConnect){
			if(eastConnect && northConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_CROSS;
			}
			else if(eastConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_T_TOP;
			}
			else{
				character = TileCharacters::Default::DOUBLE_PIPE_T_RIGHT;
			}
		}
		else{
			character = details.neCorner;
		}
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Room::DrawNECorner");
		throw 0;
	}
}


void Room::DrawSECorner(Map* map, Point point)  {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		int connectionsEast[] =  { TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT, TileCharacters::Default::DOUBLE_PIPE_T_RIGHT, TileCharacters::Default::DOUBLE_PIPE_T_TOP};
		int connectionsSouth[] = { TileCharacters::Default::DOUBLE_PIPE_VERTICAL, TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT , TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM, TileCharacters::Default::DOUBLE_PIPE_CROSS};

		bool eastConnect = std::find(std::begin(connectionsEast), std::end(connectionsEast), map->GetCharacter(point.getX() + 1, point.getY())) != std::end(connectionsEast);
		bool southConnect = std::find(std::begin(connectionsSouth), std::end(connectionsSouth), map->GetCharacter(point.getX(), point.getY() + 1) ) != std::end(connectionsSouth);

		int character;
		if(eastConnect || southConnect){
			if(eastConnect && southConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_CROSS;
			}
			else if(eastConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM;
			}
			else{
				character = TileCharacters::Default::DOUBLE_PIPE_T_RIGHT;
			}
		}
		else{
			character = details.seCorner;
		}
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Room::DrawSECorner");
		throw 0;
	}
}

void Room::DrawSWCorner(Map* map, Point point)  {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		int connectionsWest[] =  { TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT};
		int connectionsSouth[] = { TileCharacters::Default::DOUBLE_PIPE_VERTICAL, TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM, TileCharacters::Default::DOUBLE_PIPE_T_RIGHT, TileCharacters::Default::DOUBLE_PIPE_T_LEFT, TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT, TileCharacters::Default::DOUBLE_PIPE_CROSS };
		bool westConnect = false;
		bool southConnect = false;
		if(point.getX() > 0)
			westConnect = std::find(std::begin(connectionsWest), std::end(connectionsWest), map->GetCharacter(point.getX() - 1, point.getY())) != std::end(connectionsWest);

		southConnect = std::find(std::begin(connectionsSouth), std::end(connectionsSouth), map->GetCharacter(point.getX(), point.getY() + 1) ) != std::end(connectionsSouth);

		int character;
		if(westConnect || southConnect){
			if(westConnect && southConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_CROSS;
			}
			else if(westConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM;
			}
			else{
				character = TileCharacters::Default::DOUBLE_PIPE_T_LEFT;
			}
		}
		else{
			character = details.swCorner;
		}
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Room::DrawSWCorner");
		throw 0;
	}
}

void Room::DrawNWCorner(Map* map, Point point)  {
	try {
		if(map == nullptr){
			LoggerWrapper::Error("Map cannot be null");
			throw 0;
		}

		int connectionsWest[] =  { TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, TileCharacters::Default::DOUBLE_PIPE_T_LEFT, TileCharacters::Default::DOUBLE_PIPE_CROSS};
		int connectionsNorth[] = { TileCharacters::Default::DOUBLE_PIPE_VERTICAL, TileCharacters::Default::DOUBLE_PIPE_T_TOP, TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT, TileCharacters::Default::DOUBLE_PIPE_T_RIGHT};

		bool westConnect = false;

		bool northConnect = false;


		if(point.getY() > 0)
			northConnect = std::find(std::begin(connectionsNorth), std::end(connectionsNorth), map->GetCharacter(point.getX(), point.getY() - 1)) != std::end(connectionsNorth);
		if(point.getX() > 0){
			westConnect = std::find(std::begin(connectionsWest), std::end(connectionsWest), map->GetCharacter(point.getX() - 1, point.getY())) != std::end(connectionsWest);
		}
		int character;
		if(westConnect || northConnect){
			if(westConnect && northConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_CROSS;
			}
			else if(westConnect){
				character = TileCharacters::Default::DOUBLE_PIPE_T_TOP;
			}
			else{
				character = TileCharacters::Default::DOUBLE_PIPE_T_LEFT;
			}
		}
		else{
			character = details.nwCorner;
		}
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Room::DrawNWCorner");
		throw 0;
	}
}

bool Room::GetWindowsEast() const {
	return windowsEast;
}

bool Room::GetWindowsNorth() const {
	return windowsNorth;
}

bool Room::GetWindowsSouth() const {
	return windowsSouth;
}

bool Room::GetWindowsWest() const {
	return windowsWest;
}
