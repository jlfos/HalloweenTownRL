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
	rng = TCODRandom::getInstance();
	map = nullptr;
}

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	try{
		int width = map->GetWidth();
		int height = map->GetHeight();
		TCODMap* roadMap = new TCODMap(width, height);
		this->map = map;
		TCODColor visible = TCODColor::grey;
		int sizeX = rng->getInt(5, 9);
		int sizeY = rng->getInt(5, 9);
		Point lotStart(10, 10);
		Point lotEnd(30, 30);
		GenerateFence(lotStart, lotEnd);
		Point buildingStart(20, 20);
		Point buildingEnd(buildingStart.getX() + sizeX, buildingStart.getY() + sizeY);
		int side = rng->getInt(0, 3);
		int roomsLeft = 4;

		GenerateRoom(buildingStart, buildingEnd, visible, (Orientation)side, Orientation::NONE, roomsLeft);
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

void RoadMapGenerator::GenerateRoom(Point start, Point end, TCODColor color, Orientation orientation, Orientation previousOrientation, int roomsLeft) {
	try{

		/**TODO
		 * You also might need to pass in the size of the room as a parameter.
		 */
		if(previousOrientation != MapGenerator::Orientation::SOUTH){
			GenerateNorthWall(start, end, color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateSouthDoor(start, end, rng);
		}
		if(previousOrientation != MapGenerator::Orientation::NORTH){
			GenerateSouthWall(start, end, color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateNorthDoor(start, end, rng);
		}
		if(previousOrientation != MapGenerator::Orientation::WEST){
			GenerateEastWall(start, end, color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateWestDoor(start, end);
		}
		if(previousOrientation != MapGenerator::Orientation::EAST){
			GenerateWestWall(start, end, color);
			if(previousOrientation == MapGenerator::Orientation::NONE)
				GenerateEastDoor(start, end, rng);
		}
		GenerateInterior(start, end, roomsLeft);

		roomsLeft--;

		if(roomsLeft == 0)
			return;
		else{ //TODO This should probably be moved into its own method

			//TODO the roadmap needs to be passed in again OR it needs to be something that is set via the map field. Look into the latter idea first.
			Orientation newDoor = FindNextDoor(start, end);
			if(previousOrientation == MapGenerator::Orientation::NONE){
				orientation = newDoor;
			}
			switch(newDoor){
			case MapGenerator::Orientation::NORTH:
			{
				Point nextEnd(end.getX() , start.getY());
				Point nextStart(nextEnd.getX() - 5, nextEnd.getY() - 5);
				GenerateSouthDoor(nextStart, nextEnd, rng);
				GenerateRoom(nextStart, nextEnd, color, MapGenerator::Orientation::NORTH, roomsLeft);
			}
			break;
			case MapGenerator::Orientation::EAST:
			{
				Point nextStart(end.getX(), start.getY());
				Point nextEnd(nextStart.getX() + 5, nextStart.getY() + 5);
				GenerateWestDoor(nextStart, nextEnd);
				GenerateRoom(nextStart, nextEnd, color, MapGenerator::Orientation::EAST, roomsLeft);
			}
			break;
			case MapGenerator::Orientation::SOUTH:
			{
				Point nextStart(start.getX(), end.getY());
				Point nextEnd(nextStart.getX() + 5, nextStart.getY() + 5);
				GenerateNorthDoor(nextStart, nextEnd, rng);
				GenerateRoom(nextStart, nextEnd, color, MapGenerator::Orientation::SOUTH, roomsLeft);
			}
			break;
			case MapGenerator::Orientation::WEST:
			{
				Point nextEnd(start.getX(), end.getY());
				Point nextStart(nextEnd.getX() - 5, nextEnd.getY() - 5);
				GenerateEastDoor(nextStart, nextEnd, rng);
				GenerateRoom(nextStart, nextEnd, color, MapGenerator::Orientation::WEST, roomsLeft);
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


void RoadMapGenerator::GenerateRoom(Point start, Point end, TCODColor color,
		Orientation orientation, int roomsLeft) {
	GenerateRoom(start, end, color, orientation, orientation, roomsLeft);
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

void RoadMapGenerator::GenerateNorthDoor(Point start, Point end, TCODRandom* rng) {
	Point door(rng->getInt(start.getX() + 1, end.getX() - 1 ), start.getY());
	GenerateDoor(door);

}

void RoadMapGenerator::GenerateEastDoor(Point start, Point end, TCODRandom* rng) {
	Point door(end.getX(), rng->getInt(start.getY() + 1, end.getY() - 1));
	GenerateDoor(door);
}

void RoadMapGenerator::GenerateSouthDoor(Point start, Point end, TCODRandom* rng) {
	Point door(rng->getInt(start.getX() + 1, end.getX() - 1), end.getY());
	GenerateDoor(door);
}

void RoadMapGenerator::GenerateWestDoor(Point start, Point end) {
	Point door(start.getX(), rng->getInt(start.getY() + 1, end.getY() - 1));
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

MapGenerator::Orientation RoadMapGenerator::FindNextDoor(Point start, Point end) {
	try{
	bool notFound = true;
	Orientation side =  (Orientation)rng->getInt(0, 3);//randomWrap.GetOrientation();
	std::set<MapGenerator::Orientation> orientationSet = {MapGenerator::Orientation::NORTH,
														  MapGenerator::Orientation::EAST,
														  MapGenerator::Orientation::WEST,
														  MapGenerator::Orientation::SOUTH};
	while(notFound){ //TODO this has the potential to go on forever, this bug needs to be solved now
		side = (Orientation)rng->getInt(0, 3); //I like the idea of having a wrapper for rand calls, especially since I am having to cast all of them here anyways
		auto search = orientationSet.find(side);
		if(search != orientationSet.end()){
			orientationSet.erase(search);
			if(FindNextDoor(start, end, side)){
					return side;
					break;
				}
		}
		else if(orientationSet.empty()){
			std::cout << "Could not find an orientation" << std::endl;
			throw 0;
		}


	}

	return side;
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

bool RoadMapGenerator::FindNextDoor(Point start, Point end, Orientation potential) {
	bool sideFlag = true;
	switch(potential){
	case MapGenerator::Orientation::NORTH:{
		for(int i = start.getX(); i <= end.getX(); i++){  //Check north
			if(map->TileHasBeenSet(Point(i, start.getY() - 1)) || map->TileHasBeenSet(Point(i, start.getY() - 5)) ){
				sideFlag = false;
				break;
			}
		}
		return sideFlag;
	}
	break;
	case MapGenerator::Orientation::SOUTH:{
		sideFlag = true;
		for(int i = start.getX(); i <= end.getX(); i++){  //Check south
			if(map->TileHasBeenSet(Point(i, end.getY() + 1)) || map->TileHasBeenSet(Point(i, end.getY() + 5)) ){
				sideFlag = false;
				break;
			}

		}
		return sideFlag;
	}
	break;
	case MapGenerator::Orientation::EAST:{
		sideFlag = true;
		for(int i = start.getY(); i <= end.getY(); i++){  //Check east
			if(map->TileHasBeenSet(Point(end.getX() + 1, i )) || map->TileHasBeenSet(Point(end.getX() + 5, i )) ){
				sideFlag = false;
				break;
			}

		}
		return sideFlag;
	}
	break;
	case MapGenerator::Orientation::WEST:{
		sideFlag = true;
		for(int i = start.getY(); i <= end.getY(); i++){  //Check west
			if(map->TileHasBeenSet(Point(start.getX() - 1, i)) || map->TileHasBeenSet(Point(start.getX() - 5, i)) ){
				sideFlag = false;
				break;
			}

		}
		return sideFlag;

	}
	break;

	default:
		std::cerr << "It broke" << std::endl;
		return false;
	}

}
