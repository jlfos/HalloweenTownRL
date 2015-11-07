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
	roadMap = nullptr;
	minSizeX = 5;
	minSizeY = 5;
	maxSizeX = 9;
	maxSizeY = 9;
}


void RoadMapGenerator::createHouse(int lotX, int lotY, TCODColor visible) {
	Point lotStart(lotX, lotY);
	Point lotEnd(lotX + 20, lotY + 20);
	DrawFence(lotStart, lotEnd);
	Point buildingStart(lotX + 10, lotY + 10);
	MapGenerator::Orientation side = randomWrap.GetOrientation();
	int roomsLeft = 6;
	int sizeX = randomWrap.getInt(minSizeX, maxSizeX);
	int sizeY = randomWrap.getInt(minSizeY, maxSizeY);
	Room initialRoom(buildingStart, sizeX, sizeY, side);
	GenerateRoom(initialRoom, visible, Orientation::NONE, roomsLeft);
	EraseFence(lotStart, lotEnd);
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
		createHouse(lotX, lotY, visible);
		createHouse(lotX + 20, lotY, visible);
		createHouse(lotX + 40, lotY, visible);

		for (int tileX = 0; tileX < width; tileX++) {
			for (int tileY = 0; tileY < height-1; tileY++) {

				//TODO this needs to be set properly for the demo and lightsource needs to be fixed

				if(map->TileHasBeenSet(tileX + tileY * 80)){
					if(map->GetCharacter(tileX, tileY) != TileCharacters::PERIOD){
						roadMap->setProperties(tileX, tileY, false, false);
						continue;
					}
					else{
						roadMap->setProperties(tileX, tileY, true, true);
					}
				}
				else{
					roadMap->setProperties(tileX, tileY, true, true);

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
						default:
							std::cerr << "Case " << flagOri << " is not currently supported" << std::endl;
							break;
					}

					if(roadFlag)
						DrawRoad(tileX, tileY, width, roadMap);
					else
					DrawGrass(tileX, tileY, width, roadMap);
				}

			}
		}
		map->actors.push(ActorFactory::CreateLampPost(65, 7));

		return roadMap;
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::Generate" << std::endl;
		throw 0;
	}
}


void RoadMapGenerator::PopulateActors(Map* map){

}

void RoadMapGenerator::DrawGrass(int x, int y, int width, TCODMap* roadMap){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::green;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, character);
}


void RoadMapGenerator::DrawRoad(int x, int y, int width, TCODMap* roadMap){
	int tileIndex = x+y*width;
	roadMap->setProperties(x, y, true, true);
	TCODColor visible = TileColors::lightGrey;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(tileIndex, visible, character);
}

void RoadMapGenerator::DrawNextDoor(Room* ra) {

	switch (ra->getOrientation()) {
	case MapGenerator::Orientation::NORTH:
		std::cout << "Next door is south" << std::endl;
		DrawSouthDoor(ra->getNWCorner(), ra->getSECorner());
		break;
	case MapGenerator::Orientation::EAST:
		std::cout << "Next door is west" << std::endl;
		DrawWestDoor(ra->getNWCorner(), ra->getSECorner());
		break;
	case MapGenerator::Orientation::SOUTH:
		std::cout << "Next door is north" << std::endl;
		DrawNorthDoor(ra->getNWCorner(), ra->getSECorner());
		break;
	case MapGenerator::Orientation::WEST:
		std::cout << "Next door is east" << std::endl;
		DrawEastDoor(ra->getNWCorner(), ra->getSECorner());
		break;
	default:
		std::cerr << "The value " << ra->getOrientation() << " is not currently supported"
				<< std::endl;
		throw 0;
	}
}

void RoadMapGenerator::DrawWalls(Orientation previousOrientation, const Room& room,
		TCODColor color) {
	if (previousOrientation != MapGenerator::Orientation::SOUTH) {
		DrawNorthWall(room.getNWCorner(), room.getNECorner(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawSouthDoor(room.getNWCorner(), room.getSECorner());
	}
	if (previousOrientation != MapGenerator::Orientation::NORTH) {
		DrawSouthWall(room.getSWCorner(), room.getSECorner(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawNorthDoor(room.getNWCorner(), room.getSECorner());
	}
	if (previousOrientation != MapGenerator::Orientation::WEST) {
		DrawEastWall(room.getNECorner(), room.getSECorner(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawWestDoor(room.getNWCorner(), room.getSECorner());
	}
	if (previousOrientation != MapGenerator::Orientation::EAST) {
		DrawWestWall(room.getNWCorner(), room.getSWCorner(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawEastDoor(room.getNWCorner(), room.getSECorner());
	}
}

int RoadMapGenerator::GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft) {
	try{


		DrawWalls(previousOrientation, room, color);

		DrawInterior(room.getNWCorner(), room.getSECorner(), roomsLeft);

		roomsLeft--;
		std::cout << "Current Start X: " << room.getNWCorner().getX() << "Current Start Y: " << room.getNWCorner().getY() << std::endl;
		std::cout << "Current End X: " << room.getSECorner().getX() << "Current End Y: " << room.getSECorner().getY() << std::endl;
		if(roomsLeft == 0)
			return 0;
		else{
			Room* ra;
			do{
				Point start = room.getNWCorner();
				Point end = room.getSECorner();

				ra = FindNextDoor(start, end);
				if(ra != nullptr){

					DrawNextDoor(ra);

					roomsLeft = GenerateRoom((*ra), color, ra->getOrientation(), roomsLeft);
				}
			}while(roomsLeft != 0 && ra != nullptr);

			return roomsLeft;
		}
	}
	catch(...){
		std::cerr << "An error occurred in RoadMapGenerator::GenerateRoom" << std::endl;
		throw 0;
	}
}

void RoadMapGenerator::DrawNorthWall(Point start, Point end, TCODColor color) {
	DrawNWCorner(start, color);
	DrawHorizontalLine(Point(start.getX() + 1, start.getY()), end, TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, color);
	DrawNECorner(end, color);
}

void RoadMapGenerator::DrawSouthWall(Point start, Point end, TCODColor color) {
	DrawSWCorner(start, color);
	DrawHorizontalLine(Point(start.getX() + 1, start.getY()), end, TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, color);
	DrawSECorner(end, color);
}

void RoadMapGenerator::DrawEastWall(Point start, Point end, TCODColor color) {
	DrawNECorner(start, color);
	DrawVerticalLine(Point(start.getX(), start.getY() + 1), end, TileCharacters::Default::DOUBLE_PIPE_VERTICAL, color);
	DrawSECorner(end, color);
}

void RoadMapGenerator::DrawWestWall(Point start, Point end, TCODColor color) {
	DrawNWCorner(start, color);
	DrawVerticalLine(Point(start.getX(), start.getY() + 1), end, TileCharacters::Default::DOUBLE_PIPE_VERTICAL, color);
	DrawSWCorner(end, color);
}


void RoadMapGenerator::DrawHorizontalLine(Point start, Point end, int character, TCODColor color) {
	for(int i = start.getX(); i < end.getX(); i++){
		map->SetTileProperties(Point(i, end.getY()), color, character);
	}
}


void RoadMapGenerator::DrawVerticalLine(Point start, Point end, int character, TCODColor color) {
	for(int i = start.getY(); i < end.getY(); i++){
		map->SetTileProperties(Point(start.getX(), i), color, character);
	}
}


void RoadMapGenerator::DrawInterior(Point start, Point end, int character) { //TODO this needs to go back to being period but I will keep it like this for debugging purposes
	try{
		//TODO I need to start looking into different kinds of rooms. ENUM_CLASS for room types?
		/*TODO Room type ideas:
		 * living room( TV, desk, chair, sofa?)
		 * dining room(table, chairs)
		 * bathroom (toilet?, bathtub? )
		 * bedroom (bed, chair, table, dresser, desk)
		 * */
		std::cout << "Interior is " << character << std::endl;
		TCODColor visible = TCODColor::grey;
		for(int i = start.getX() + 1 ; i < end.getX(); i++){
			for(int j = start.getY() + 1 ; j < end.getY(); j++){
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(Point(i, j), visible, character);
			}
		}
	}
	catch(...){
		std::cerr << "An error occurred in GenerateInterior" << std::endl;
		throw 0;
	}
}


void RoadMapGenerator::DrawDoor(const Point& door) {
	TCODColor color = TCODColor::grey;
	int character = TileCharacters::Default::PERIOD;
	map->SetTileProperties(door, color, character);
}

void RoadMapGenerator::DrawNorthDoor(Point start, Point end) {
	Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1 ), start.getY());
	DrawDoor(door);

}

void RoadMapGenerator::DrawEastDoor(Point start, Point end) {
	Point door(end.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
	std::cout << "East Door X: " << door.getX() << "Y: "  << door.getY() << std::endl;
	DrawDoor(door);
}

void RoadMapGenerator::DrawSouthDoor(Point start, Point end) {
	Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1), end.getY());
	DrawDoor(door);
}

void RoadMapGenerator::DrawWestDoor(Point start, Point end) {
	Point door(start.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
	std::cout << "West Door X: " << door.getX() << "Y: "  << door.getY() << std::endl;
	DrawDoor(door);
}

void RoadMapGenerator::DrawNECorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
	map->SetTileProperties(point, color, character);
}

void RoadMapGenerator::DrawSECorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
	map->SetTileProperties(point, color, character);
}

void RoadMapGenerator::DrawSWCorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
	map->SetTileProperties(point, color, character);
}

void RoadMapGenerator::DrawNWCorner(Point point, TCODColor color) {
	int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
	map->SetTileProperties(point, color, character);
}

Room* RoadMapGenerator::FindNextDoor(Point start, Point end) {
	try{
	bool notFound = true;
	Room* r = nullptr;
	Orientation side;
	std::set<MapGenerator::Orientation> orientationSet = {MapGenerator::Orientation::NORTH,
														  MapGenerator::Orientation::EAST,
														  MapGenerator::Orientation::WEST,
														  MapGenerator::Orientation::SOUTH};
	while(notFound){
		side = randomWrap.GetOrientation();
		auto search = orientationSet.find(side);
		if(search != orientationSet.end()){
			orientationSet.erase(search);

			r = FindNextDoor(start, end, side);
			if(r != nullptr){
					break;
			}
		}
		else if(orientationSet.empty()){
			std::cout << "Could not find an orientation" << std::endl;
			break;
		}
	}
	return r;
	}
	catch(...){
		std::cerr << "An error occurred in FindNextDoor" << std::endl;
		throw 0;
	}
}

void RoadMapGenerator::DrawFence(Point start, Point end) {
	DrawNorthWall(start, Point(end.getX(), start.getY()), TCODColor::white);
	DrawSouthWall(Point(start.getX(), end.getY()), end, TCODColor::white);
	DrawEastWall(Point(end.getX(), start.getY()), end, TCODColor::white);
	DrawWestWall(start, Point(start.getX(), end.getY()), TCODColor::white);
}

//TODO Finish erase fence
void RoadMapGenerator::EraseFence(Point start, Point end) {
	Room ra(start, end, MapGenerator::Orientation::NORTH);
	DrawHorizontalLine(ra.getNWCorner(), ra.getNECorner(), TileCharacters::RAINBOW, TCODColor::white);
	DrawVerticalLine(ra.getNECorner(), ra.getSECorner(), TileCharacters::RAINBOW, TCODColor::white);
	DrawHorizontalLine(ra.getSWCorner(), ra.getSECorner(), TileCharacters::RAINBOW, TCODColor::white);
	DrawVerticalLine(ra.getNWCorner(), ra.getSWCorner(), TileCharacters::RAINBOW, TCODColor::white);
	map->SetTileProperties(ra.getSECorner(), TCODColor::white, TileCharacters::RAINBOW);

}

//TODO There are still cases where a room will overwrite another room.
Room* RoadMapGenerator::FindNextDoor(Point start, Point end, Orientation potential) {
	switch(potential){
	case MapGenerator::Orientation::NORTH:{
		int y = 1; //TODO Clean this function up. Its really ugly.
//I think you could break each of these case statements out into their own methods. Those methods might be broken into generate offset
// and generate points/room. Point generation is done a case by case basis so I don't think that is something that can be made generic.
		for(; y < maxSizeY && y < start.getY(); y++){  //Check north
			if(map->TileHasBeenSet(Point(end.getX() , start.getY() - y))){
				break;
			}
		}
		y--;

		if(y < minSizeY)
			return nullptr;

		int x = 1;
		for(; x <= maxSizeX && x < start.getX(); x++){
			if(map->TileHasBeenSet(Point(end.getX() - x, start.getY() - y))){
				break;
			}
		}

		x--;
		if(x < minSizeX)
			return nullptr;

		int temp = y;
		for(; temp > 1; temp--){
			if(map->TileHasBeenSet(Point(end.getX() - x, start.getY() - temp))){
				return nullptr;
			}
		}

		int offsetX = randomWrap.getInt(minSizeX, x);
		int offsetY = randomWrap.getInt(minSizeY, y);

		Point newEnd(end.getX(), start.getY());
		Point newStart(newEnd.getX() - offsetX, newEnd.getY() - offsetY);
		Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::NORTH);
		return r;

	}
	break;
	case MapGenerator::Orientation::SOUTH:{
		int y = 1;
		for(; y < maxSizeY; y++){  //Check south
			if(map->TileHasBeenSet(Point(start.getX() , end.getY() + y))){
				break;
			}
		}
		y--;

		if(y < minSizeY)
			return nullptr;

		int x = 1;
		for(; x <= maxSizeX; x++){
			if(map->TileHasBeenSet(Point(start.getX() + x, end.getY() + y))){
				break;
			}
		}

		x--;
		if(x < minSizeX)
			return nullptr;

		int temp = y;
		for(; temp > 1; temp--){
			if(map->TileHasBeenSet(Point(start.getX() + x, end.getY() + temp))){
				return nullptr;
			}
		}

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
		for(; x <= maxSizeX; x++){ //Check east
			if(map->TileHasBeenSet(Point(end.getX() + x, start.getY()))){
				break;
			}
		}
		x--;
		if(x < minSizeX)
			return nullptr;

		int y = 1;
		for(; y < maxSizeY; y++){  //Check southeast
			if(map->TileHasBeenSet(Point(end.getX() + x , start.getY() + y))){
				break;
			}
		}
		y--;

		if(y < minSizeY)
			return nullptr;

		int temp = x;
		for(; temp > 1; temp--){  //Check southeast
			if(map->TileHasBeenSet(Point(end.getX() + temp , start.getY() + y))){
				return nullptr;
			}
		}

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
		for(; x <= maxSizeX && x < start.getX(); x++){
			if(map->TileHasBeenSet(Point(start.getX() - x, end.getY()))){
				break;
			}
		}

		x--;
		if(x < minSizeX)
			return nullptr;

		int y = 1;
		for(; y < maxSizeY && y < start.getY(); y++){  //Check west
			if(map->TileHasBeenSet(Point(start.getX() - x, end.getY() - y))){
				break;
			}
		}
		y--;

		if(y < minSizeY)
			return nullptr;


		int temp = x;
		for(; temp > 1; temp--){  //Check west
			if(map->TileHasBeenSet(Point(start.getX() - temp, end.getY() - y))){
				return nullptr;
			}
		}




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
		throw 0;
	}

}


