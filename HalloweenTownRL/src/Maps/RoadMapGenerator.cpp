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
#include "../LoggerWrapper.hpp"

#ifndef RMG_LOGGER
//#define RMG_LOGGER
#endif

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


void RoadMapGenerator::CreateHouse(int lotX, int lotY, TCODColor visible) {
	try {
#ifdef RMG_LOGGER
		LoggerWrapper::Debug("Creating House");
#endif
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
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::CreateHouse");
		throw 0;
	}
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
		CreateHouse(lotX, lotY, visible);
		CreateHouse(lotX + 20, lotY, visible);
		CreateHouse(lotX + 40, lotY, visible);

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height-1; y++) {
				//TODO this needs to be set properly for the demo and lightsource needs to be fixed

				if(map->TileHasBeenSet(x, y)){
					if(map->GetCharacter(x, y) != TileCharacters::PERIOD){
						roadMap->setProperties(x, y, false, false);
						continue;
					}
					else{
						roadMap->setProperties(x, y, true, true);
					}
				}
				else{
					roadMap->setProperties(x, y, true, true);

					bool roadFlag = false;
					switch(flagOri){
						case MapGenerator::Orientation::NORTH:
						case MapGenerator::Orientation::SOUTH:
							roadFlag = x <= (width/2)+3  &&  x >= (width/2)-3;
							break;
						case MapGenerator::Orientation::EAST:
						case MapGenerator::Orientation::WEST:
							roadFlag = y <= (height/2)+3  &&  y >= (height/2)-3;
							break;
						default:
							LoggerWrapper::Error("Case " + std::to_string(flagOri) + " is not currently supported");
							break;
					}

					if(roadFlag)
						DrawRoad(x, y, roadMap);
					else
					DrawGrass(x, y, roadMap);
				}

			}
		}
		map->actors.push(ActorFactory::CreateLampPost(65, 7));

		return roadMap;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::Generate");
		throw 0;
	}
}


void RoadMapGenerator::PopulateActors(Map* map){

}

void RoadMapGenerator::DrawGrass(int x, int y, TCODMap* roadMap){
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


void RoadMapGenerator::DrawRoad(int x, int y, TCODMap* roadMap){
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

void RoadMapGenerator::DrawNextDoor(Room* ra) {
	try {
		switch (ra->getOrientation()) {
		case MapGenerator::Orientation::NORTH:
			#ifdef RMG_LOGGER
			LoggerWrapper::Info( "Next door is south" );
			#endif
			DrawSouthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::EAST:
			#ifdef RMG_LOGGER
			LoggerWrapper::Info("Next door is west");
			#endif
			DrawWestDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::SOUTH:
			#ifdef RMG_LOGGER
			LoggerWrapper::Info("Next door is north");
			#endif
			DrawNorthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::WEST:
			#ifdef RMG_LOGGER
			LoggerWrapper::Info("Next door is east");
			#endif
			DrawEastDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		default:
			LoggerWrapper::Error("The value " + std::to_string(ra->getOrientation()) + " is not currently supported");
			throw 0;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawNextDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawWalls(Orientation previousOrientation, const Room& room,
		TCODColor color) {
	try {
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
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawWalls");
		throw 0;
	}
}

int RoadMapGenerator::GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft) {
	try{
		DrawWalls(previousOrientation, room, color);

		DrawInterior(room.getNWCorner(), room.getSECorner(), roomsLeft);

		roomsLeft--;
#ifdef RMG_LOGGER
		LoggerWrapper::Debug("Current Start X: " + std::to_string(room.getNWCorner().getX()) + "Current Start Y: " + std::to_string(room.getNWCorner().getY()));
		LoggerWrapper::Debug("Current End X: " + std::to_string(room.getSECorner().getX()) + "Current End Y: " + std::to_string(room.getSECorner().getY()));
#endif
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
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::GenerateRoom");
		throw 0;
	}
}

void RoadMapGenerator::DrawNorthWall(Point start, Point end, TCODColor color) {
	try {
		DrawNWCorner(start, color);
		DrawHorizontalLine(Point(start.getX() + 1, start.getY()), end, TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, color);
		DrawNECorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawNorthWall");
		throw 0;
	}
}

void RoadMapGenerator::DrawSouthWall(Point start, Point end, TCODColor color) {
	try {
		DrawSWCorner(start, color);
		DrawHorizontalLine(Point(start.getX() + 1, start.getY()), end, TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, color);
		DrawSECorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawSouthWall");
		throw 0;
	}
}

void RoadMapGenerator::DrawEastWall(Point start, Point end, TCODColor color) {
	try {
		DrawNECorner(start, color);
		DrawVerticalLine(Point(start.getX(), start.getY() + 1), end, TileCharacters::Default::DOUBLE_PIPE_VERTICAL, color);
		DrawSECorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawEastWall");
		throw 0;
	}
}

void RoadMapGenerator::DrawWestWall(Point start, Point end, TCODColor color) {
	try {
		DrawNWCorner(start, color);
		DrawVerticalLine(Point(start.getX(), start.getY() + 1), end, TileCharacters::Default::DOUBLE_PIPE_VERTICAL, color);
		DrawSWCorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawWestWall");
		throw 0;
	}
}


void RoadMapGenerator::DrawHorizontalLine(Point start, Point end, int character, TCODColor color) {
	try {
		for(u_int i = start.getX(); i <= end.getX(); i++){
			map->SetTileProperties(Point(i, end.getY()), color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawHorizontalLine");
		throw 0;
	}
}


void RoadMapGenerator::DrawVerticalLine(Point start, Point end, int character, TCODColor color) {
	try {
		for(u_int i = start.getY(); i <= end.getY(); i++){
			map->SetTileProperties(Point(start.getX(), i), color, character);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawVerticalLine");
		throw 0;
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
#ifdef RMG_LOGGER
		LoggerWrapper::Debug("Interior is " +  std::to_string(character) );
#endif
		TCODColor visible = TCODColor::grey;
		for(uint i = start.getX() + 1 ; i < end.getX(); i++){
			for(uint j = start.getY() + 1 ; j < end.getY(); j++){
				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(Point(i, j), visible, character);
			}
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::GenerateInterior");
		throw 0;
	}
}


void RoadMapGenerator::DrawDoor(const Point& door) {
	try {
		TCODColor color = TCODColor::grey;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(door, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawNorthDoor(Point start, Point end) {
	try {
		Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1 ), start.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawNorthDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawEastDoor(Point start, Point end) {
	try {
		Point door(end.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawEastDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawSouthDoor(Point start, Point end) {
	try {
		Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1), end.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawSouthDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawWestDoor(Point start, Point end) {
	try {
		Point door(start.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawWestDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawNECorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawNECorner");
		throw 0;
	}
}

void RoadMapGenerator::DrawSECorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawSECorner");
		throw 0;
	}
}

void RoadMapGenerator::DrawSWCorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawSWCorner");
		throw 0;
	}
}

void RoadMapGenerator::DrawNWCorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawNWCorner");
		throw 0;
	}
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
#ifdef RMG_LOGGER
			LoggerWrapper::Debug("Could not find an orientation");
#endif
			break;
		}
	}
	return r;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::FindNextDoor");
		throw 0;
	}
}

void RoadMapGenerator::DrawFence(Point start, Point end) {
	try {
		DrawNorthWall(start, Point(end.getX(), start.getY()), TCODColor::white);
		DrawSouthWall(Point(start.getX(), end.getY()), end, TCODColor::white);
		DrawEastWall(Point(end.getX(), start.getY()), end, TCODColor::white);
		DrawWestWall(start, Point(start.getX(), end.getY()), TCODColor::white);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::DrawFence");
		throw 0;
	}
}

//TODO Finish erase fence
void RoadMapGenerator::EraseFence(Point start, Point end) {
	try {
		Room ra(start, end, MapGenerator::Orientation::NORTH);
		DrawHorizontalLine(ra.getNWCorner(), ra.getNECorner(), TileCharacters::RAINBOW, TCODColor::white);
		DrawVerticalLine(ra.getNECorner(), ra.getSECorner(), TileCharacters::RAINBOW, TCODColor::white);
		DrawHorizontalLine(ra.getSWCorner(), ra.getSECorner(), TileCharacters::RAINBOW, TCODColor::white);
		DrawVerticalLine(ra.getNWCorner(), ra.getSWCorner(), TileCharacters::RAINBOW, TCODColor::white);
		map->SetTileProperties(ra.getSECorner(), TCODColor::white, TileCharacters::RAINBOW);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::EraseFence");
		throw 0;
	}

}

//TODO There are still cases where a room will overwrite another room.
Room* RoadMapGenerator::FindNextDoor(Point start, Point end, Orientation potential) {
	try {
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
			LoggerWrapper::Error("It broke");
			throw 0;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in RoadMapGenerator::FindNextDoor");
		throw 0;
	}

}


