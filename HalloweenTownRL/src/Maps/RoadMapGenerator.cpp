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


RoadMapGenerator::RoadMapGenerator():RoadMapGenerator(MapGenerator::Orientation::NORTH) {

}
RoadMapGenerator::RoadMapGenerator(MapGenerator::Orientation orientation){
	this->flagOri = orientation;
	map = nullptr;
	minSizeX = 5;
	minSizeY = 5;
	maxSizeX = 9;
	maxSizeY = 9;
}

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	try{
		int width = map->GetWidth();
		int height = map->GetHeight();
		TCODMap* roadMap = new TCODMap(width, height);
		this->map = map;
		TCODColor visible = TCODColor::grey;
		int sizeX = randomWrap.getInt(minSizeX, maxSizeX);
		int sizeY = randomWrap.getInt(minSizeY, maxSizeY);
		Point lotStart(10, 10);
		Point lotEnd(30, 30);
		GenerateFence(lotStart, lotEnd);
		Point buildingStart(20, 20);
		MapGenerator::Orientation side = (MapGenerator::Orientation)randomWrap.getInt(0, 3);
		int roomsLeft = 2;
		Room initialRoom(buildingStart, sizeX, sizeY, side);
		GenerateRoom(initialRoom ,visible, Orientation::NONE, roomsLeft);
		for (int tileX = 0; tileX < width; tileX++) {
			for (int tileY = 0; tileY < height-1; tileY++) {
				roadMap->setProperties(tileX, tileY, true, true);
				if(map->TileHasBeenSet(tileX + tileY * 80))
					continue;

				bool roadFlag = false;
				switch(flagOri){
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
					GenerateRoad(tileX, tileY, width, roadMap);
				else
					GenerateGrass(tileX, tileY, width, roadMap);


			}
		}
		map->actors.push(ActorFactory::CreateLampPost(19, 19));

		return roadMap;
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::Generate" << std::endl;
		throw 0;
	}
}


void RoadMapGenerator::PopulateActors(Map* map){

}

void RoadMapGenerator::GenerateGrass(int x, int y, int width, TCODMap* roadMap){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::green;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, character);
}


void RoadMapGenerator::GenerateRoad(int x, int y, int width, TCODMap* roadMap){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::lightGrey;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, character);
}

void RoadMapGenerator::GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft) {
	try{

		/**TODO
		 * You also might need to pass in the size of the room as a parameter.
		 */
		if(previousOrientation != MapGenerator::Orientation::SOUTH){
			GenerateNorthWall(room.getStart(), room.getEnd(), color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateSouthDoor(room.getStart(), room.getEnd());
		}
		if(previousOrientation != MapGenerator::Orientation::NORTH){
			GenerateSouthWall(room.getStart(), room.getEnd(), color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateNorthDoor(room.getStart(), room.getEnd());
		}
		if(previousOrientation != MapGenerator::Orientation::WEST){
			GenerateEastWall(room.getStart(), room.getEnd(), color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateWestDoor(room.getStart(), room.getEnd());
		}
		if(previousOrientation != MapGenerator::Orientation::EAST){
			GenerateWestWall(room.getStart(), room.getEnd(), color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateEastDoor(room.getStart(), room.getEnd());
		}
		GenerateInterior(room.getStart(), room.getEnd(), roomsLeft);

		roomsLeft--;

		if(roomsLeft == 0)
			return;
		else{ //TODO This should probably be moved into its own method

			Point end = room.getEnd();
			Point start = room.getStart();
			//TODO the roadmap needs to be passed in again OR it needs to be something that is set via the map field. Look into the latter idea first.
			Room* ra = FindNextDoor(room.getStart(), room.getEnd());
			switch(ra->getOrientation()){
			case MapGenerator::Orientation::NORTH:
			{
				GenerateSouthDoor(ra->getStart(), ra->getEnd());
				GenerateRoom(ra->getStart(), ra->getEnd(), color, MapGenerator::Orientation::NORTH, roomsLeft);
			}
			break;
			case MapGenerator::Orientation::EAST:
			{
				GenerateWestDoor(ra->getStart(), ra->getEnd());
				GenerateRoom(ra->getStart(), ra->getEnd(), color, MapGenerator::Orientation::EAST, roomsLeft);
			}
			break;
			case MapGenerator::Orientation::SOUTH:
			{
				GenerateNorthDoor(ra->getStart(), ra->getEnd());
				GenerateRoom(ra->getStart(), ra->getEnd(), color, MapGenerator::Orientation::SOUTH, roomsLeft);
			}
			break;
			case MapGenerator::Orientation::WEST:
			{
				GenerateEastDoor(ra->getStart(), ra->getEnd());
				GenerateRoom(ra->getStart(), ra->getEnd(), color, MapGenerator::Orientation::WEST, roomsLeft);
			}
			break;
			default:
				std::cerr << "The value " << ra->getOrientation() << " is not currently supported" << std::endl;
				throw 0;
				break;
			}
		}
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::GenerateRoom" << std::endl;
	}
}


void RoadMapGenerator::GenerateRoom(Point start, Point end, TCODColor color,
		Orientation orientation, int roomsLeft) {
	Room room(start, end, orientation);
	GenerateRoom(room, color, orientation, roomsLeft);
}

void RoadMapGenerator::GenerateNorthWall(Point start, Point end, TCODColor color) {
	int character = TileCharacters::Default::RAINBOW;
	GenerateNWCorner(start, color);
	for(int i = start.getX() + 1; i < end.getX(); i++){
		character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
		map->SetTileProperties(Point(i, start.getY()), color, character);
	}
	GenerateNECorner(Point(end.getX(), start.getY()), color);
}

void RoadMapGenerator::GenerateSouthWall(Point start, Point end, TCODColor color) {
		int character = TileCharacters::Default::RAINBOW;
		GenerateSWCorner(Point(start.getX(), end.getY()), color);
		for(int i = start.getX() + 1; i < end.getX(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
			map->SetTileProperties(Point(i, end.getY()), color, character);
		}
		GenerateSECorner(end, color);
}

void RoadMapGenerator::GenerateEastWall(Point start, Point end, TCODColor color) {
		int character = TileCharacters::Default::RAINBOW;
		GenerateNECorner(Point(end.getX(), start.getY()), color);
		for(int i = start.getY() + 1; i < end.getY(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			map->SetTileProperties(Point(end.getX(), i), color, character);
		}
		GenerateSECorner(end, color);
}

void RoadMapGenerator::GenerateWestWall(Point start, Point end, TCODColor color) {
		int character = TileCharacters::Default::RAINBOW;
		GenerateNWCorner(start, color);

		for(int i = start.getY() + 1; i < end.getY(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			map->SetTileProperties(Point(start.getX(), i), color, character);
		}
		GenerateSWCorner(Point(start.getX(), end.getY()), color);
}

void RoadMapGenerator::GenerateInterior(Point start, Point end, int character) { //TODO this needs to go back to being period but I will keep it like this for debugging purposes
	try{
		//TODO I need to start looking into different kinds of rooms. ENUM_CLASS for room types?
		/*TODO Room type ideas:
		 * living room( TV, desk, chair, sofa?)
		 * dining room(table, chairs)
		 * bathroom (toilet?, bathtub? )
		 * bedroom (bed, chair, table, dresser, desk)
		 * */

		TCODColor visible = TCODColor::grey;
		for(int i = start.getX() + 1 ; i < end.getX(); i++){
			for(int j = start.getY() + 1 ; j < end.getY(); j++){
//				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(Point(i, j), visible, character);
			}
		}
	}
	catch(...){
		std::cerr << "An error occurred in GenerateInterior" << std::endl;
		throw 0;
	}
}


void RoadMapGenerator::GenerateDoor(const Point& door) {
	TCODColor color = TCODColor::grey;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(door, color, character);
}

void RoadMapGenerator::GenerateNorthDoor(Point start, Point end) {
	Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1 ), start.getY());
	GenerateDoor(door);

}

void RoadMapGenerator::GenerateEastDoor(Point start, Point end) {
	Point door(end.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
	GenerateDoor(door);
}

void RoadMapGenerator::GenerateSouthDoor(Point start, Point end) {
	Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1), end.getY());
	GenerateDoor(door);
}

void RoadMapGenerator::GenerateWestDoor(Point start, Point end) {
	Point door(start.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
	GenerateDoor(door);
}

void RoadMapGenerator::GenerateNECorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
	map->SetTileProperties(point, color, character);
}

void RoadMapGenerator::GenerateSECorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
	map->SetTileProperties(point, color, character);
}

void RoadMapGenerator::GenerateSWCorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
	map->SetTileProperties(point, color, character);
}

void RoadMapGenerator::GenerateNWCorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
	map->SetTileProperties(point, color, character);
}

Room* RoadMapGenerator::FindNextDoor(Point start, Point end) {
	try{
	bool notFound = true;
	Orientation side;// =  (Orientation)randomWrap.getInt(0, 3);//randomWrap.GetOrientation();
	std::set<MapGenerator::Orientation> orientationSet = {MapGenerator::Orientation::NORTH,
														  MapGenerator::Orientation::EAST,
														  MapGenerator::Orientation::WEST,
														  MapGenerator::Orientation::SOUTH};
	while(notFound){ //TODO this has the potential to go on forever, this bug needs to be solved now
		side = MapGenerator::Orientation::WEST; //I like the idea of having a wrapper for rand calls, especially since I am having to cast all of them here anyways
		auto search = orientationSet.find(side);
		if(search != orientationSet.end()){
//			orientationSet.erase(search);
			Room* r = FindNextDoor(start, end, side);
			if(r != nullptr){
					return r;
					break;
				}
		}
		else if(orientationSet.empty()){
			std::cout << "Could not find an orientation" << std::endl;
			throw 0;
		}


	}

	return nullptr;
	}
	catch(...){
		std::cerr << "An error occurred in FindNextDoor" << std::endl;
		throw 0;
	}
}

void RoadMapGenerator::GenerateFence(Point start, Point end) {
	GenerateNorthWall(start, end, TCODColor::white);
	GenerateSouthWall(start, end, TCODColor::white);
	GenerateEastWall(start, end, TCODColor::white);
	GenerateWestWall(start, end, TCODColor::white);
}

Room* RoadMapGenerator::FindNextDoor(Point start, Point end, Orientation potential) {
	switch(potential){
	case MapGenerator::Orientation::NORTH:{
		int y = 1;
		for(; y < maxSizeY; y++){  //Check north
			if(map->TileHasBeenSet(Point(start.getX() , start.getY() - y))){
				break;
			}
		}
		y--;

		if(y<minSizeY)
			return nullptr;

		int x = 1;
		for(; x <= maxSizeX; x++){
			if(map->TileHasBeenSet(Point(start.getX() + x, start.getY() - y))){
				break;
			}
		}

		x--;
		if(x<minSizeX)
			return nullptr;

		int offsetX = randomWrap.getInt(minSizeX, x);
		int offsetY = randomWrap.getInt(minSizeY, y);
		Point newEnd(end.getX(), start.getY());
		Point newStart(newEnd.getX()-offsetX, newEnd.getY()-offsetY);
		Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::NORTH);
		return r;

	}
	break;
	case MapGenerator::Orientation::SOUTH:{
		int y = 1;
		for(; y < maxSizeY; y++){  //Check north
			if(map->TileHasBeenSet(Point(start.getX() , end.getY() + y))){

				break;

			}
		}
		y--;

		if(y<minSizeY)
			return nullptr;

		int x = 1;
		for(; x <= maxSizeX; x++){
			if(map->TileHasBeenSet(Point(start.getX() + x, end.getY() + y))){
				break;
			}
		}

		x--;
		if(x<minSizeX)
			return nullptr;

		int offsetX = randomWrap.getInt(minSizeX, x);
		int offsetY = randomWrap.getInt(minSizeY, y);

		Point newStart(start.getX(), end.getY());
		Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);
		Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::SOUTH);
		return r;

	}
	break;
	case MapGenerator::Orientation::EAST:{


		int x = 1;
		for(; x <= maxSizeX; x++){
			if(map->TileHasBeenSet(Point(end.getX() + x, start.getY()))){
				break;
			}
		}

		x--;
		if(x<minSizeX)
			return nullptr;

		int y = 1;
		for(; y < maxSizeY; y++){  //Check north
			if(map->TileHasBeenSet(Point(end.getX() + x , start.getY() + y))){

				break;

			}
		}
		y--;

		if(y<minSizeY)
			return nullptr;

		int offsetX = randomWrap.getInt(minSizeX, x);
		int offsetY = randomWrap.getInt(minSizeY, y);

		Point newStart(end.getX(), start.getY());
		Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);
		Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::EAST);
		return r;

	}
	break;
	case MapGenerator::Orientation::WEST:{
		int x = 1;
		for(; x <= maxSizeX; x++){
			if(map->TileHasBeenSet(Point(start.getX() - x, start.getY()))){
				break;
			}
		}

		x--;
		if(x<minSizeX)
			return nullptr;

		int y = 1;
		for(; y < maxSizeY; y++){  //Check north
			if(map->TileHasBeenSet(Point(start.getX() - x, start.getY() - y))){
				break;
			}
		}
		y--;

		if(y<minSizeY)
			return nullptr;

		int offsetX = randomWrap.getInt(minSizeX, x);
		int offsetY = randomWrap.getInt(minSizeY, y);
		Point newEnd(start.getX(), end.getY());
		Point newStart(newEnd.getX() - offsetX, newEnd.getY() - offsetY);
		Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::WEST);
		return r;

	}
	break;

	default:
		std::cerr << "It broke" << std::endl;
	}

}
