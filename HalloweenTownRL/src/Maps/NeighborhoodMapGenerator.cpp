#include <string>
#include <iostream>
#include <memory>
#include <set>

#include "libtcod.hpp"

#include "Map.hpp"
#include "MapGenerator.hpp"
#include "NeighborhoodMapGenerator.hpp"
#include "../Tile/TileColors.hpp"
#include "../LoggerWrapper.hpp"

#ifndef NMG_LOGGER
#define NMG_LOGGER
#endif

NeighborhoodMapGenerator::NeighborhoodMapGenerator(int width, int height,
		MapGenerator::Orientation orientation): mapWidth(width), mapHeight(height), flagOri(orientation) {
	minRoomSizeX = 4;
	minRoomSizeY = 4;
	maxRoomSizeX = 9;
	maxRoomSizeY = 9;

}

void NeighborhoodMapGenerator::PopulateActors(Map* map) {
}


TCODMap* NeighborhoodMapGenerator::Generate(Map* map, bool generateActors) {
	try{
		int width = map->GetWidth();
		int height = map->GetHeight();
		neighborhoodMap = new TCODMap(width, height);
		this->map = map;
		TCODColor visible = TCODColor::grey;

		//TODO This is as close to 0,0 as I can go. I get exceptions otherwise. I need to look into this
		int lotX = 0;
		int lotY = 0;
		CreateHouse(lotX, lotY, visible);
		CreateHouse(lotX + 20, lotY, visible);
		CreateHouse(lotX + 40, lotY, visible);

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height-1; y++) {
				//TODO this needs to be set properly for the demo and lightsource needs to be fixed

				if(map->TileHasBeenSet(x, y)){
					if(map->GetCharacter(x, y) != TileCharacters::PERIOD){
						neighborhoodMap->setProperties(x, y, true, true);
						continue;
					}
					else{
						neighborhoodMap->setProperties(x, y, true, true);
					}
				}
				else{
					neighborhoodMap->setProperties(x, y, true, true);

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
						DrawRoad(x, y, neighborhoodMap);
					else
					DrawGrass(x, y, neighborhoodMap);
				}

			}
		}
		map->actors.push(ActorFactory::CreateLampPost(75, 7));

		return neighborhoodMap;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::Generate");
		throw 0;
	}
}


void NeighborhoodMapGenerator::DrawGrass(int x, int y, TCODMap* roadMap){
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


void NeighborhoodMapGenerator::DrawRoad(int x, int y, TCODMap* roadMap){
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

void NeighborhoodMapGenerator::EraseFence(Point start, Point end) {
	try {
		Room ra(start, end, MapGenerator::Orientation::NORTH);
		DrawHorizontalLine(map, ra.getNWCorner(), ra.getNECorner(), TileCharacters::RAINBOW, TCODColor::white);
		DrawVerticalLine(map, ra.getNECorner(), ra.getSECorner(), TileCharacters::RAINBOW, TCODColor::white);
		DrawHorizontalLine(map, ra.getSWCorner(), ra.getSECorner(), TileCharacters::RAINBOW, TCODColor::white);
		DrawVerticalLine(map, ra.getNWCorner(), ra.getSWCorner(), TileCharacters::RAINBOW, TCODColor::white);
		map->SetTileProperties(ra.getSECorner(), TCODColor::white, TileCharacters::RAINBOW);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::EraseFence");
		throw 0;
	}
}



Room* NeighborhoodMapGenerator::FindNextDoor(Point start, Point end, Orientation potential) {
	try {
		switch(potential){
		case MapGenerator::Orientation::NORTH:{
			Point tempStart(end.getX() , start.getY());
			bool xNeg = true;
			bool yNeg = true;

			Point tempEnd = CheckVerticalRoom(tempStart, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(start);
				xNeg = false;
				tempEnd = CheckVerticalRoom(tempStart, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);
			Room* r;
			if(xNeg){
				Point newEnd(end.getX(), start.getY());
				Point newStart(newEnd.getX() - offsetX, newEnd.getY() - offsetY);
				r = new Room(newStart, newEnd, MapGenerator::Orientation::NORTH);
			}
			else{
				Point newStart(start.getX(), start.getY() - offsetY);
				Point newEnd(start.getX() + offsetX , start.getY());
				r = new Room(newStart, newEnd, MapGenerator::Orientation::NORTH);
			}
			return r;

		}
		break;
		case MapGenerator::Orientation::SOUTH:{
			Point tempStart(start.getX() , end.getY());
			bool xNeg = false;
			bool yNeg = false;

			Point tempEnd = CheckVerticalRoom(tempStart, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(end);
				xNeg = true;
				tempEnd = CheckVerticalRoom(tempStart, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);

			Point newStart(start.getX(), end.getY());
			Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);


			Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::SOUTH);

			return r;

		}
		break;
		case MapGenerator::Orientation::EAST:{

			Point tempStart(end.getX(), start.getY());
			bool xNeg = false;
			bool yNeg = false;
			Point tempEnd = CheckHorizontalRoom(tempStart, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(end);
				yNeg = true;
				tempEnd = CheckHorizontalRoom(tempStart, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);

			Room* r = nullptr;
			if(yNeg){
				Point newStart(tempStart.getX(), tempEnd.getY());
				Point newEnd(tempEnd.getX(), tempStart.getY());
				r = new Room(newStart, newEnd, MapGenerator::Orientation::EAST);
			}
			else{
				Point newStart(end.getX(), start.getY());
				Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);
				r = new Room(newStart, newEnd, MapGenerator::Orientation::EAST);
			}

			return r;

		}
		break;
		case MapGenerator::Orientation::WEST:{
			Point tempStart(start.getX(), end.getY());
			bool xNeg = true;
			bool yNeg = true;
			Point tempEnd = CheckHorizontalRoom(tempStart, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(start);
				yNeg = false;
				tempEnd = CheckHorizontalRoom(tempStart, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());
			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);

			Room* r = nullptr;
			if(yNeg){
				Point newEnd = Point(start.getX(), end.getY());
				Point newStart(newEnd.getX() - offsetX, newEnd.getY() - offsetY);
				r = new Room(newStart, newEnd, MapGenerator::Orientation::WEST);
			}
			else{
				Point newEnd = Point(tempStart.getX(), tempEnd.getY());
				Point newStart(newEnd.getX() - offsetX, newEnd.getY() - offsetY);
				r = new Room(newStart, newEnd, MapGenerator::Orientation::WEST);
			}

			return r;
		}
		break;

		default:
			LoggerWrapper::Error("It broke");
			throw 0;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::FindNextDoor");
		throw 0;
	}
}


void NeighborhoodMapGenerator::DrawDoor(const Point& door) {
	try {
		TCODColor color = TCODColor::grey;
		int character = TileCharacters::Default::PERIOD;
		map->SetTileProperties(door, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawNorthDoor(Point start, Point end) {
	try {
		bool validDoor = true;//false;
		int x;
		do{
			x = randomWrap.getInt(start.getX() + 1, end.getX() - 1 );

//			if(ValidNSDoor(x, start.getY())){
//				validDoor = true;
//			}

		} while(!validDoor);
		Point door(x, start.getY());

		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNorthDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawEastDoor(Point start, Point end) {
	try {
		int y;
		bool validDoor = true;//false;
		do{
			y = randomWrap.getInt(start.getY() + 1, end.getY() - 1);
//			if(ValidEWDoor(end.getX(), y)){
//				validDoor = true;
//			}
		} while(!validDoor);
		Point door(end.getX(), y );
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawEastDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawWestDoor(Point start, Point end) {
	try {
		int y;
		bool validDoor = true;//false;
		do{
			y = randomWrap.getInt(start.getY() + 1, end.getY() - 1);
//			if(ValidEWDoor(start.getX(), y)){
//				validDoor = true;
//			}
		} while(!validDoor);
		Point door(start.getX(), y);
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawWestDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawSouthDoor(Point start, Point end) {
	try {
		int x;
		bool validDoor = true;//false;
		do{
			x = randomWrap.getInt(start.getX() + 1, end.getX() - 1);
//			if(ValidNSDoor(x, end.getY())){
//				validDoor = true;
//			}

		}while(!validDoor);

		Point door(x, end.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSouthDoor");
		throw 0;
	}
}



void NeighborhoodMapGenerator::DrawNECorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_RIGHT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNECorner");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawSECorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_RIGHT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSECorner");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawSWCorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_LOWER_LEFT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSWCorner");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawNWCorner(Point point, TCODColor color) {
	try {
		int character = TileCharacters::Default::DOUBLE_PIPE_CORNER_UPPER_LEFT;
		map->SetTileProperties(point, color, character);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNWCorner");
		throw 0;
	}
}

Room* NeighborhoodMapGenerator::FindNextDoor(Point start, Point end) {
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
#ifdef NMG_LOGGER
			LoggerWrapper::Debug("Could not find an orientation");
#endif
			break;
		}
	}
	return r;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::FindNextDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawFence(Point start, Point end) {
	try {
		DrawNorthWall(start, Point(end.getX(), start.getY()), TCODColor::white);
		DrawSouthWall(Point(start.getX(), end.getY()), end, TCODColor::white);
		DrawEastWall(Point(end.getX(), start.getY()), end, TCODColor::white);
		DrawWestWall(start, Point(start.getX(), end.getY()), TCODColor::white);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawFence");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawInterior(Point start, Point end, int character) { //TODO this needs to go back to being period but I will keep it like this for debugging purposes
	try{
		//TODO I need to start looking into different kinds of rooms. ENUM_CLASS for room types?
		/*TODO Room type ideas:
		 * living room( TV, desk, chair, sofa?)
		 * dining room(table, chairs)
		 * bathroom (toilet?, bathtub? )
		 * bedroom (bed, chair, table, dresser, desk)
		 * */
#ifdef NMG_LOGGER
		LoggerWrapper::Debug("Interior is " +  std::to_string(character) );
#endif
		TCODColor visible = TCODColor::grey;
		for(uint i = start.getX() + 1 ; i < end.getX(); i++){
			for(uint j = start.getY() + 1 ; j < end.getY(); j++){
//				int character = TileCharacters::Default::PERIOD;
				map->SetTileProperties(Point(i, j), visible, character);
			}
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::GenerateInterior");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawNorthWall(Point start, Point end, TCODColor color) {
	try {
		DrawNWCorner(start, color);
		DrawHorizontalLine(map, Point(start.getX() + 1, start.getY()), end, TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, color);
		DrawNECorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNorthWall");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawSouthWall(Point start, Point end, TCODColor color) {
	try {
		DrawSWCorner(start, color);
		DrawHorizontalLine(map, Point(start.getX() + 1, start.getY()), end, TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL, color);
		DrawSECorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSouthWall");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawEastWall(Point start, Point end, TCODColor color) {
	try {
		DrawNECorner(start, color);
		DrawVerticalLine(map, Point(start.getX(), start.getY() + 1), end, TileCharacters::Default::DOUBLE_PIPE_VERTICAL, color);
		DrawSECorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawEastWall");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawWestWall(Point start, Point end, TCODColor color) {
	try {
		DrawNWCorner(start, color);
		DrawVerticalLine(map, Point(start.getX(), start.getY() + 1), end, TileCharacters::Default::DOUBLE_PIPE_VERTICAL, color);
		DrawSWCorner(end, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawWestWall");
		throw 0;
	}
}

int NeighborhoodMapGenerator::GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft) {
	try{
		DrawWalls(previousOrientation, room, color);
		DrawNextDoor(&room);
		DrawInterior(room.getNWCorner(), room.getSECorner(), roomsLeft);

		roomsLeft--;
#ifdef NMG_LOGGER
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



					roomsLeft = GenerateRoom((*ra), color, ra->getOrientation(), roomsLeft);
				}
			}while(roomsLeft != 0 && ra != nullptr);

			return roomsLeft;
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::GenerateRoom");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawNextDoor(Room* ra) {
	try {
		switch (ra->getOrientation()) {
		case MapGenerator::Orientation::NORTH:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info( "Next door is north" );
			#endif
			DrawSouthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::EAST:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is east");
			#endif
			DrawWestDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::SOUTH:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is south");
			#endif
			DrawNorthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::WEST:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is west");
			#endif
			DrawEastDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		default:
			LoggerWrapper::Error("The value " + std::to_string(ra->getOrientation()) + " is not currently supported");
			throw 0;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNextDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawWalls(Orientation previousOrientation, const Room& room,
		TCODColor color) {
	try {
		DrawNorthWall(room.getNWCorner(), room.getNECorner(), color);
		DrawSouthWall(room.getSWCorner(), room.getSECorner(), color);
		DrawEastWall(room.getNECorner(), room.getSECorner(), color);
		DrawWestWall(room.getNWCorner(), room.getSWCorner(), color);
		if (previousOrientation != MapGenerator::Orientation::SOUTH) {

			if (previousOrientation == MapGenerator::Orientation::NONE)
				DrawNorthDoor(room.getNWCorner(), room.getSECorner());

		}
		if (previousOrientation != MapGenerator::Orientation::NORTH) {

			if (previousOrientation == MapGenerator::Orientation::NONE)
				DrawNorthDoor(room.getNWCorner(), room.getSECorner());
		}
		if (previousOrientation != MapGenerator::Orientation::WEST) {

			if (previousOrientation == MapGenerator::Orientation::NONE)
				DrawWestDoor(room.getNWCorner(), room.getSECorner());
		}
		if (previousOrientation != MapGenerator::Orientation::EAST) {

			if (previousOrientation == MapGenerator::Orientation::NONE)
				DrawEastDoor(room.getNWCorner(), room.getSECorner());
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawWalls");
		throw 0;
	}
}

void NeighborhoodMapGenerator::CreateHouse(int lotX, int lotY, TCODColor visible) {
	try {
#ifdef NMG_LOGGER
		LoggerWrapper::Debug("Creating House");
#endif
		Point lotStart(lotX, lotY);
		Point lotEnd(lotX + 20, lotY + 20);
		DrawFence(lotStart, lotEnd);
		Point buildingStart(lotX + 10, lotY + 10);
		MapGenerator::Orientation side = randomWrap.GetOrientation();
		int roomsLeft = 6;
		int sizeX = randomWrap.getInt(minRoomSizeX, maxRoomSizeX);
		int sizeY = randomWrap.getInt(minRoomSizeY, maxRoomSizeY);
		Room initialRoom(buildingStart, sizeX, sizeY, side);
		GenerateRoom(initialRoom, visible, Orientation::NONE, roomsLeft);
//		EraseFence(lotStart, lotEnd);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CreateHouse");
		throw 0;
	}
}

Point NeighborhoodMapGenerator::CheckHorizontalRoom(Point start, bool xNegFlag, bool yNegFlag) {
	try {
		int xCof,yCof;

		if(xNegFlag)
			xCof = -1;
		else
			xCof = 1;

		if(yNegFlag)
			yCof = -1;
		else
			yCof = 1;

		int x = 1;
		for(; x <= maxRoomSizeX; x++){ //Check east
			if(map->TileHasBeenSet(start.getX() + (x * xCof), start.getY()) ||
					start.getX() + (x * xCof) == 0 ||
					start.getX() + (x * xCof) == mapWidth - 1){
				break;
			}
		}
		x--;
		LoggerWrapper::Debug(" CVH, east/west check complete");
		if(x < minRoomSizeX)
			return start;

		int y = 1;
		for(; y <= maxRoomSizeY; y++){  //Check southeast
			if(map->TileHasBeenSet(start.getX() + (x * xCof) , start.getY() + (y * yCof)) ||
					start.getY() + (y * yCof) == 0 ||
					start.getY() + (y * yCof) == mapHeight - 2){
				break;
			}
		}
		y--;
		LoggerWrapper::Debug(" CVH, north/south check complete");
		if(y < minRoomSizeY)
			return start;

		int tempY = 0;
		for(; tempY < y; tempY++ ){
			if(map->TileHasBeenSet(start.getX() + (1 * xCof), start.getY() + (tempY * yCof))){
				tempY--;
				if(tempY < minRoomSizeY){
					return start;
				}
				else{
					break;
				}
			}
		}
		y = tempY;
		LoggerWrapper::Debug(" CVH complete");
		return Point(start.getX() + (x * xCof) , start.getY() + (y * yCof));
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CheckHorizontalRoom");
		throw 0;
	}
}

bool NeighborhoodMapGenerator::ValidEWDoor(const int x, const int y) {
	bool validDoor = false;
	int leftChar = map->GetCharacter(x - 1, y);
	int rightChar = map->GetCharacter(x + 1, y);
	LoggerWrapper::Debug("Left Char " + std::to_string(leftChar) + " " + "Right Char" + std::to_string(rightChar));
	if((rightChar == TileCharacters::Default::PERIOD ||
			(rightChar == TileCharacters::Default::RAINBOW)) &&
			(leftChar == TileCharacters::Default::PERIOD ||
					leftChar == TileCharacters::Default::RAINBOW)){
		validDoor = true;
	}
	return validDoor;
}

bool NeighborhoodMapGenerator::ValidNSDoor(const int x, const int y) {
	bool validDoor = false;
	if( (map->GetCharacter(x, y + 1 ) == TileCharacters::Default::PERIOD ||
			map->GetCharacter(x, y + 1 ) == TileCharacters::Default::RAINBOW) &&
			(map->GetCharacter(x, y - 1 ) == TileCharacters::Default::PERIOD ||
					map->GetCharacter(x, y - 1 ) == TileCharacters::Default::RAINBOW)){
		validDoor = true;
	}
	return validDoor;

}

Point NeighborhoodMapGenerator::CheckVerticalRoom(Point start, bool xNegFlag, bool yNegFlag) {
	try {
		LoggerWrapper::Debug("CVR start: " + start.ToString());
		int xCof,yCof;

		if(xNegFlag)
			xCof = -1;
		else
			xCof = 1;

		if(yNegFlag)
			yCof = -1;
		else
			yCof = 1;

		int y = 1;
		for(; y <= maxRoomSizeY; y++){ //Check east
			if(map->TileHasBeenSet(start.getX(), start.getY() + (y * yCof)) ||
					start.getY() + (y * yCof) == 0 || start.getY() + (y * yCof) == mapHeight - 2){
				break;
			}
		}
		y--;
		LoggerWrapper::Debug("CVR, north/south check complete " + std::to_string(y));
		if(y < minRoomSizeY)
			return start;

		int x = 1;
		for(; x <= maxRoomSizeX; x++){  //Check southeast
			if(map->TileHasBeenSet(start.getX() + (x * xCof) , start.getY() + (y * yCof))  ||
					start.getX() + (x * xCof) == 0 || start.getX() + (x * xCof) == mapWidth - 1){
				break;
			}
		}
		x--;

		if(x < minRoomSizeX)
			return start;

		LoggerWrapper::Debug("CVR, east/west check complete " + std::to_string(x));
		int tempX = 0;
		for(; tempX < x; tempX++ ){
			if(map->TileHasBeenSet(start.getX() + (tempX * xCof), start.getY() + (1 * yCof))){
				tempX--;
				if(tempX < minRoomSizeX){
					return start;
				}
				else{
					break;
				}
			}
		}

		x = tempX;
		Point tempEnd = Point(start.getX() + (x * xCof), start.getY() + (y * yCof));
		LoggerWrapper::Debug(" CVR complete: " + tempEnd.ToString());
		return tempEnd;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CheckVerticalRoom");
		throw 0;
	}

}

bool NeighborhoodMapGenerator::InvalidRoomCorners(Point start, Point end) {
	bool result = start.getX() == end.getX() || start.getY() == end.getY() ||
			start.getX() == 0 || start.getY() == 0 ||
			end.getX() == 0 || end.getY() == 0 ||
			start.getX() >= mapWidth - 1 || start.getY() >= mapHeight - 2 ||
			end.getX() >= mapWidth - 1 || end.getY() >= mapHeight - 2;
	return result;
}
