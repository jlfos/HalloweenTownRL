/*
 * RoadMapGenerator.cpp
 *
 *  Created on: Feb 28, 2015
 */
#include <iostream>
#include <memory>
#include "libtcod.hpp"
#include "Map.hpp"
#include "MapGenerator.hpp"
#include "RoadMapGenerator.hpp"
#include "../Tile/TileColors.hpp"


RoadMapGenerator::RoadMapGenerator():RoadMapGenerator(MapGenerator::Orientation::NORTH) {

}
RoadMapGenerator::RoadMapGenerator(MapGenerator::Orientation orientation){
	this->orientation = orientation;
	rng = TCODRandom::getInstance();
}

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	try{
		int width = map->GetWidth();
		int height = map->GetHeight();
		TCODMap* roadMap = new TCODMap(width, height);

		for (int tileX = 0; tileX < width; tileX++) {
			for (int tileY = 0; tileY < height-1; tileY++) {
				bool roadFlag = false;
				switch(orientation){
					case MapGenerator::Orientation::NORTH:
					case MapGenerator::Orientation::SOUTH:
						roadFlag = tileX <= (width/2)+3  &&  tileX >= (width/2)-3;
						break;
					case MapGenerator::Orientation::EAST:
					case MapGenerator::Orientation::WEST:
						roadFlag = tileY <= (height/2)+3  &&  tileY >= (height/2)-3;
						break;
				}

				if(roadFlag)
					GenerateRoad(tileX, tileY, width, roadMap, map);
				else
					GenerateGrass(tileX, tileY, width, roadMap, map);


			}
		}
		map->actors.push(ActorFactory::CreateLampPost(19, 19));
		TCODColor visible = TCODColor::grey;
		int sizeX = rng->getInt(5, 9);
		int sizeY = rng->getInt(5, 9);
		Point start(20, 20);
		Point end(start.getX() + sizeX, start.getY() + sizeY);
		int side = rng->getInt(0, 3);
		int roomsLeft = 3;

		GenerateRoom(map, roadMap, start, end, visible, (Orientation)side, roomsLeft);

		return roadMap;
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::Generate" << std::endl;
		throw 0;
	}
}


void RoadMapGenerator::PopulateActors(Map* map){

}

void RoadMapGenerator::GenerateGrass(int x, int y, int width, TCODMap* roadMap, Map* map){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::green;
	TCODColor fog = TileColors::darkerGreen;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, fog, character);
}


void RoadMapGenerator::GenerateRoad(int x, int y, int width, TCODMap* roadMap, Map* map){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::lightGrey;
	TCODColor fog = TileColors::darkGrey;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, fog, character);
}

void RoadMapGenerator::GenerateRoom(Map* map, TCODMap* cityMap, Point start, Point end, TCODColor color, Orientation side, int roomsLeft) {
	try{

		/**TODO You need either an orientation (probably the easier way) or you need the location of the door from the previous room
		 * You also might need to pass in the size of the room as a parameter and startX/startY really need to be combined into a
		 * Point object.
		 */
		GenerateNorthWall(start, end, color, map);
		GenerateSouthWall(start, end, color, map);
		GenerateEastWall(start, end, color, map);
		GenerateWestWall(start, end, color, map);
		GenerateInterior(start, end, map);
		GenerateDoor(start, end, rng, map, side);
		roomsLeft--;
		if(roomsLeft == 0)
			return;
		else{
			Orientation newDoor = NORTH;
			switch(newDoor){
			case NORTH:
			{
				Point nextStart(end.getX(), start.getY());
				Point nextEnd(nextStart.getX() - 5, nextStart.getY() - 5);
				GenerateRoom(map, cityMap, nextEnd, nextStart, color, (Orientation)newDoor, roomsLeft);
			}
			break;
			case EAST:
			{
				Point nextStart(end.getX(), start.getY());
				Point nextEnd(nextStart.getX() + 5, nextStart.getY() + 5);
				GenerateRoom(map, cityMap, nextStart, nextEnd, color, (Orientation)newDoor, roomsLeft);
			}
			break;
			case SOUTH:
			{
				Point nextStart(end.getX(), end.getY());
				Point nextEnd(nextStart.getX() - 5, nextStart.getY() + 5);
				GenerateRoom(map, cityMap, nextStart, nextEnd, color, (Orientation)newDoor, roomsLeft);
			}
			break;
			case WEST:
			{
				Point nextStart(end.getX(), start.getY());
				Point nextEnd(nextStart.getX() - 5, nextStart.getY() - 5);
				GenerateRoom(map, cityMap, nextStart, nextEnd, color, (Orientation)newDoor, roomsLeft);
			}
			break;
			default:
				std::cerr << "The value " << newDoor << " is not currently supported" << std::endl;
				throw 0;
				break;
			}


		}
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::GenerateRoom" << std::endl;
	}
}


void RoadMapGenerator::GenerateNorthWall(Point start, Point end, TCODColor color, Map* map) {
	int character = TileCharacters::Default::RAINBOW;
	GenerateNWCorner(start, map, color);
	for(int i = start.getX() + 1; i < end.getX(); i++){
		character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
		map->SetTileProperties(Point(i, start.getY()), color, color, character);
	}
	GenerateNECorner(Point(end.getX(), start.getY()), map, color);
}

void RoadMapGenerator::GenerateSouthWall(Point start, Point end, TCODColor color, Map* map) {
		int character = TileCharacters::Default::RAINBOW;
		GenerateSECorner(Point(start.getX(), end.getY()), map, color);
		for(int i = start.getX() + 1; i < end.getX(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
			map->SetTileProperties(Point(i, end.getY()), color, color, character);
		}
		GenerateSWCorner(end, map, color);
}

void RoadMapGenerator::GenerateEastWall(Point start, Point end, TCODColor color, Map* map) {
		int character = TileCharacters::Default::RAINBOW;
		GenerateNECorner(Point(end.getX(), start.getY()), map, color);
		for(int i = start.getY() + 1; i < end.getY(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			map->SetTileProperties(Point(end.getX(), i), color, color, character);
		}
		GenerateSECorner(end, map, color);
}

void RoadMapGenerator::GenerateWestWall(Point start, Point end, TCODColor color, Map* map) {
		int character = TileCharacters::Default::RAINBOW;
		GenerateNWCorner(start, map, color);

		for(int i = start.getY() + 1; i < end.getY(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			map->SetTileProperties(Point(start.getX(), i), color, color, character);
		}
		GenerateSWCorner(Point(start.getX(), end.getY()), map, color);
}

void RoadMapGenerator::GenerateInterior(Point start, Point end, Map* map) {
	try{

		TCODColor visible = TCODColor::grey;
		TCODColor fog = TCODColor::darkGrey;
		for(int i = start.getX() + 1 ; i < end.getX(); i++){
			for(int j = start.getY() + 1 ; j < end.getY(); j++){
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(Point(i, j), visible, fog, character);
			}
		}
	}
	catch(...){
		std::cerr << "An error occurred in GenerateInterior" << std::endl;
		throw 0;
	}
}

void RoadMapGenerator::GenerateDoor(Point start, Point end, TCODRandom* rng, Map* map, Orientation side ) {
	try{
		switch(side){
		case Orientation::NORTH:
			{
				Point door(rng->getInt(start.getX() + 1, end.getX() - 1 ), start.getY());
				TCODColor color = TCODColor::grey;
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(door, color, color, character);
			}
			break;
		case Orientation::EAST:
			{
				Point door(end.getX(), rng->getInt(start.getY() + 1, end.getY() - 1));
				TCODColor color = TCODColor::grey;
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(door, color, color, character);
			}
			break;
		case Orientation::SOUTH:
			{
				Point door(rng->getInt(start.getX() + 1, end.getX() - 1), end.getY());
				TCODColor color = TCODColor::grey;
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(door, color, color, character);
			}
			break;
		case Orientation::WEST:
			{
				Point door(start.getX(), rng->getInt(start.getY() + 1, end.getY() - 1));
				TCODColor color = TCODColor::grey;
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(door, color, color, character);
			}
			break;
		default:
			std::cerr << "The value " << side << " is not supported" << std::endl;
			throw 0;
		}
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::GenerateDoorPoint" << std::endl;
		throw 0;
	}
}

void RoadMapGenerator::GenerateNECorner(Point point, Map* map, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
	map->SetTileProperties(point, color, color, character);
}

void RoadMapGenerator::GenerateSECorner(Point point, Map* map, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
	map->SetTileProperties(point, color, color, character);
}

void RoadMapGenerator::GenerateSWCorner(Point point, Map* map, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
	map->SetTileProperties(point, color, color, character);
}

void RoadMapGenerator::GenerateNWCorner(Point point, Map* map, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
	map->SetTileProperties(point, color, color, character);
}

int RoadMapGenerator::FindNextDoor(Orientation currentDoor) {
	bool notFound = true;
	int side = rng->getInt(0, 3);
	while(notFound){
		if(side == currentDoor)
			side = rng->getInt(0, 3);
		else
			return side;
	}
}
