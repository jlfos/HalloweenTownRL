/*
 * Lot.cpp
 *
 *  Created on: Jan 2, 2016
 */
#include <algorithm>
#include <set>

#include "../LoggerWrapper.hpp"
#include "Lot.hpp"
#include "Map.hpp"
#include "Generators/MapGenerator.hpp"
#include "Point.hpp"
#include "../Tile/TileColors.hpp"
#include "../RandomWrapper.hpp"
#include "Room.hpp"
const int LOT_SIZE_X = 26;
const int LOT_SIZE_Y = 14;
const int MIN_ROOM_SIZE_X = 4;
const int MAX_ROOM_SIZE_X = 7;
const int MIN_ROOM_SIZE_Y = 4;
const int MAX_ROOM_SIZE_Y = 7;
const int LOT_DESIGNATOR = TileCharacters::Default::DOUBLE_EXCLAMATION;


Lot::Lot(TCODMap *neighborhoodMap, Map *map, RandomWrapper *randomWrap, LotPosition position):
		randomWrap(randomWrap), position(position), lotStart(0,0), lotEnd(0,0), map(map), mapWidth(map->GetWidth()), mapHeight(map->GetHeight()),neighborhoodMap(neighborhoodMap), backDoor(false) {
	Point tempStart(0, 3);

	furniture.push_back(TileCharacters::Default::BED);
	furniture.push_back(TileCharacters::Default::TV);
	furniture.push_back(TileCharacters::Default::TABLE);
	furniture.push_back(TileCharacters::Default::CHAIR);
	int yStreetSize = 7;
	switch(position){
	case LotPosition::NORTHWEST:{
		lotStart = Point(tempStart);
		lotEnd = Point(lotStart, LOT_SIZE_X, LOT_SIZE_Y );

		break;
	}
	case LotPosition::NORTH: {
		lotStart = Point(tempStart, LOT_SIZE_X, 0); //Shift start point 1 right
		lotEnd = Point(lotStart, LOT_SIZE_X, LOT_SIZE_Y);
		break;
	}
	case LotPosition::NORTHEAST:{
		lotStart = Point(tempStart, LOT_SIZE_X * 2, 0); //Shift start point 2 right
		lotEnd = Point(lotStart, LOT_SIZE_X, LOT_SIZE_Y);
		break;
	}
	case LotPosition::SOUTHEAST:{
		lotStart = Point(tempStart, LOT_SIZE_X * 2, (LOT_SIZE_Y * 1) + yStreetSize ); //Shift start 2 right, 1 down
		lotEnd = Point(lotStart, LOT_SIZE_X, LOT_SIZE_Y);
		break;
	}
	case LotPosition::SOUTH:{
		lotStart = Point(tempStart, LOT_SIZE_X, (LOT_SIZE_Y * 1) + yStreetSize ); //Shift start 1 right, 1 down
		lotEnd = Point(lotStart, LOT_SIZE_X, LOT_SIZE_Y);
		break;
	}
	case LotPosition::SOUTHWEST:{
		lotStart = Point(tempStart, 0, (LOT_SIZE_Y * 1) + yStreetSize ); //Shift start 1 down
		lotEnd = Point(lotStart, LOT_SIZE_X, LOT_SIZE_Y);
		break;
	}
	case LotPosition::WEST:
	default:
		LoggerWrapper::Error("An incorrect orientation was passed to the switch statement.");
		throw 0;
		break;

	};
}

Lot::~Lot() {
}

void Lot::PopulateLot() {
	try {
		int ySection;
		MapGenerator::DrawRectangle(map, lotStart, lotEnd, TileColors::white, LOT_DESIGNATOR);
		int xSectionCount = LOT_SIZE_X / (MIN_ROOM_SIZE_X); //Break into sections based off the minimum room size
		int xSection = randomWrap->getInt(0, xSectionCount - 1); //Randomly selects which X section will be used
		int xOffset = MIN_ROOM_SIZE_X * xSection;
		xOffset = std::max(1, xOffset); //Calculates the offset and makes sure that it is at least 1
		int ySectionCount = LOT_SIZE_Y / (MIN_ROOM_SIZE_X);
		MapGenerator::Orientation side;
		switch(position){
		case LotPosition::NORTHWEST:
		case LotPosition::NORTH:
		case LotPosition::NORTHEAST:{
			ySection = 0;
			side = MapGenerator::Orientation::SOUTH;
			break;
		}
		case LotPosition::SOUTHEAST:
		case LotPosition::SOUTH:
		case LotPosition::SOUTHWEST:{
			ySection = ySectionCount - 1;
			side = MapGenerator::Orientation::NORTH;
			break;
		}
		case LotPosition::WEST:
		default:
			LoggerWrapper::Error("An incorrect orientation was passed to the switch statement.");
			throw 0;
			break;

		};

		int yOffset = MIN_ROOM_SIZE_X * ySection;
		yOffset = std::max(1, yOffset);

		Point buildingStart(lotStart, xOffset, yOffset);

		LoggerWrapper::Debug("StartX: " + std::to_string(buildingStart.getX()) + " StartY: " + std::to_string(buildingStart.getY()));

		int roomsLeft = 6;

		//lot start + lot size (aka end of lot)
		int maxXSize = std::min( (int)((lotStart.getX() + LOT_SIZE_X) - (lotStart.getX()  + xOffset)), MAX_ROOM_SIZE_X);
		int maxYSize = std::min( (int)((lotStart.getY() + LOT_SIZE_Y) - (lotStart.getY() + yOffset)), MAX_ROOM_SIZE_Y);
		//	int maxXSize = std::min( (lotStart.getX() + LOT_SIZE_X) - (lotStart.getX()  + xOffset), MAX_ROOM_SIZE_X);
		//	int maxYSize = std::min( (lotStart.getY() + LOT_SIZE_Y) - (lotStart.getY() + yOffset), MAX_ROOM_SIZE_Y);
		Point buildingEnd(lotStart);
		do{
			int sizeX = randomWrap->getInt(MIN_ROOM_SIZE_X, maxXSize);
			int sizeY = randomWrap->getInt(MIN_ROOM_SIZE_X, maxYSize);
			buildingEnd = Point(buildingStart, sizeX, sizeY);
		}while(map->TileHasBeenSet(buildingEnd));

		Room initialRoom(buildingStart, buildingEnd, side);

		GenerateRoom(initialRoom, TileColors::white, roomsLeft);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::PopulateLot");
		throw 0;
	}

}

int Lot::GenerateRoom(Room room, TCODColor color, int roomsLeft) {

	try{
		room.Draw(map, false);
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

				ra = FindNextDoor(room);
				if(ra != nullptr){



					roomsLeft = GenerateRoom((*ra), color, roomsLeft);
				}
			}while(roomsLeft != 0 && ra != nullptr);
			delete ra;
			return roomsLeft;
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in Lot::GenerateRoom");
		throw 0;
	}
}

void Lot::DrawNextDoor(Room* ra) {
	try {
		switch (ra->getOrientation()) {
		case MapGenerator::Orientation::NORTH:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info( "Next door is north of the previous room" );
			#endif
			DrawSouthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::EAST:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is east of the previous room");
			#endif
			DrawWestDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::SOUTH:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is south of the previous room");
			#endif
			DrawNorthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::WEST:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is west of the previous room");
			#endif
			DrawEastDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		default:
			LoggerWrapper::Error("The value " + std::to_string(ra->getOrientation()) + " is not currently supported");
			throw 0;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawNextDoor");
		throw 0;
	}
}

void Lot::DrawDoor(const Point& door) {
	try {
		map->actors.push(ActorFactory::CreateDoor(door.getX(), door.getY()));
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawDoor");
		throw 0;
	}
}

void Lot::DrawNorthDoor(Point start, Point end) {
	try {
		bool validDoor = false;
		int x;
		do{
			x = randomWrap->getInt(start.getX() + 1, end.getX() - 1 );

			if(ValidDoor(x, start.getY())){
				validDoor = true;
			}

		} while(!validDoor);
		Point door(x, start.getY());

		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawNorthDoor");
		throw 0;
	}
}

void Lot::DrawEastDoor(Point start, Point end) {
	try {
		int y;
		bool validDoor = false;
		do{
			y = randomWrap->getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(end.getX(), y)){
				validDoor = true;
			}
		} while(!validDoor);
		Point door(end.getX(), y );
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawEastDoor");
		throw 0;
	}
}

void Lot::DrawSouthDoor(Point start, Point end) {
	try {
		int x;
		bool validDoor = false;
		do{
			x = randomWrap->getInt(start.getX() + 1, end.getX() - 1);
			if(ValidDoor(x, end.getY())){
				validDoor = true;
			}

		}while(!validDoor);

		Point door(x, end.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawSouthDoor");
		throw 0;
	}
}

void Lot::DrawWestDoor(Point start, Point end) {
	try {
		int y;
		bool validDoor = false;
		do{
			y = randomWrap->getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(start.getX(), y)){
				validDoor = true;
			}
		} while(!validDoor);
		Point door(start.getX(), y);
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawWestDoor");
		throw 0;
	}
}

bool Lot::ValidDoor(const int x, const int y) {
	try {
		bool validDoor = false;
		LoggerWrapper::Debug("Door X:" + std::to_string(x) + " Y:" + std::to_string(y));
		int character =  map->GetCharacter(x, y);
		std::vector<int> invalidCharacters = {TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM, TileCharacters::Default::DOUBLE_PIPE_T_LEFT, TileCharacters::Default::DOUBLE_PIPE_T_RIGHT, TileCharacters::Default::DOUBLE_PIPE_T_TOP };
		validDoor = (std::find(std::begin(invalidCharacters), std::end(invalidCharacters), character) == std::end(invalidCharacters));

		LoggerWrapper::Debug("Character of door tile " + std::to_string(character) + " is valid " + std::to_string(validDoor));
		return validDoor;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::ValidDoor");
		throw 0;
	}
}

void Lot::DrawInterior(Point start, Point end, int character) {
	try{
		//TODO I need to start looking into different kinds of rooms. ENUM_CLASS for room types?
		/*TODO Room type ideas:
		 * living room( TV, desk, chair, sofa?)
		 * dining room(table, chairs)
		 * bathroom (toilet?, bathtub? )
		 * bedroom (bed, chair, table, dresser, desk)
		 * */
		int furnitureRoom = 2;
		//TODO Move this into the Room class so that the interior can be filled prior to deletion but after all doors have been created
		//TODO Make a toilet
		//TODO Try to make a tub
		//TODO try to make a desk
		//TODO Try to make a sink
		//TODO Get colors for individual furniture types & add potted plants (music note)
#ifdef NMG_LOGGER
		LoggerWrapper::Debug("Interior is " +  std::to_string(character) );
#endif
		TCODColor visible = TCODColor::grey;
		for(uint i = start.getX() + 1 ; i < end.getX(); i++){
			for(uint j = start.getY() + 1 ; j < end.getY(); j++){
				int character;

				int tempChar = randomWrap->getInt(i + j, end.getX() + end.getY());
				if(map->GetCharacter(i - 1, j) != TileCharacters::Default::PLUS &&
						map->GetCharacter(i + 1, j) != TileCharacters::Default::PLUS &&
						map->GetCharacter(i, j + 1) != TileCharacters::Default::PLUS &&
						map->GetCharacter(i, j - 1) != TileCharacters::Default::PLUS &&
						furnitureRoom != 0 && tempChar == end.getX() + end.getY()){
					furnitureRoom--;
					character = furniture.at(randomWrap->getInt(0, 3));
					visible = TileColors::brownLighter;
				}
				else{
					character = TileCharacters::Default::PERIOD;
					visible = TileColors::grey;
				}
				map->SetTileProperties(Point(i, j), visible, character);
			}
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in Lot::GenerateInterior");
		throw 0;
	}
}

Room* Lot::FindNextDoor(Room room) {
	try{
	bool notFound = true;
	Room* r = nullptr;
	MapGenerator::Orientation side; //TODO Change
	std::set<MapGenerator::Orientation> orientationSet = {MapGenerator::Orientation::NORTH,
														  MapGenerator::Orientation::EAST,
														  MapGenerator::Orientation::WEST,
														  MapGenerator::Orientation::SOUTH};
	while(notFound){
		side = randomWrap->GetOrientation();
		auto search = orientationSet.find(side);
		if(search != orientationSet.end()){
			orientationSet.erase(search);

			r = FindNextDoor(room, side);
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
		LoggerWrapper::Error("An error occurred in Lot::FindNextDoor");
		throw 0;
	}
}

Room* Lot::FindNextDoor(Room room, MapGenerator::Orientation potential) {
	try {
		Point start = room.getNWCorner();
		Point end = room.getSECorner();
		switch(potential){
		case MapGenerator::Orientation::NORTH:{
			Point tempStart(end.getX() , start.getY());
			bool xNeg = true;
			bool yNeg = true;

			Point tempEnd(0, 0);
			CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(start);
				xNeg = false;
				Lot::RoomCheckResult result = CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == Lot::RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, MapGenerator::Orientation::NORTH);
					}
					if(result == RoomCheckResult::NO_SPACE_LOT_1 && !backDoor){
						DrawNorthDoor(room.getNWCorner(), room.getSECorner());
						backDoor = true;
					}
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap->getInt(MIN_ROOM_SIZE_X, x);
			int offsetY = randomWrap->getInt(MIN_ROOM_SIZE_X, y);
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

			Point tempEnd(0, 0);
			CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(end);
				xNeg = true;
				Lot::RoomCheckResult result = CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, MapGenerator::Orientation::SOUTH);
					}
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap->getInt(MIN_ROOM_SIZE_X, x);
			int offsetY = randomWrap->getInt(MIN_ROOM_SIZE_X, y);
			Room* r;
			if(xNeg){
				Point newStart(tempStart.getX() - offsetX, tempStart.getY());
				Point newEnd(tempStart.getX(), tempStart.getY() + offsetY);
				r = new Room(newStart, newEnd, MapGenerator::Orientation::SOUTH);
			}
			else{
				Point newStart(start.getX(), end.getY());
				Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);
				r = new Room(newStart, newEnd, MapGenerator::Orientation::SOUTH);
			}

			return r;

		}
		break;
		case MapGenerator::Orientation::EAST:{

			Point tempStart(end.getX(), start.getY());
			bool xNeg = false;
			bool yNeg = false;
			Point tempEnd(0,0);
			CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(end);
				yNeg = true;
				Lot::RoomCheckResult result = CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, MapGenerator::Orientation::EAST);
					}
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap->getInt(MIN_ROOM_SIZE_X, x);
			int offsetY = randomWrap->getInt(MIN_ROOM_SIZE_X, y);

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
			Point tempEnd(0,0);
			CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(start);
				yNeg = false;
				Lot::RoomCheckResult result = CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, MapGenerator::Orientation::WEST);
					}
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());
			int offsetX = randomWrap->getInt(MIN_ROOM_SIZE_X, x);
			int offsetY = randomWrap->getInt(MIN_ROOM_SIZE_X, y);

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
		LoggerWrapper::Error("An error occurred in Lot::FindNextDoor");
		throw 0;
	}
}

Lot::RoomCheckResult Lot::CheckHorizontalRoom(Point start, Point& end, bool xNegFlag, bool yNegFlag) {
	try {
		int xCof,yCof;
		bool lotHit = false;
		if(xNegFlag)
			xCof = -1;
		else
			xCof = 1;

		if(yNegFlag)
			yCof = -1;
		else
			yCof = 1;

		int x = 1;
		for (; x <= MAX_ROOM_SIZE_X; x++) {
			Point currentPoint = Point(start.getX() + (x * xCof), start.getY());
			//Check east
			if (map->TileHasBeenSet(currentPoint) ||
					start.getX() + (x * xCof) == 0 ||
					start.getX() + (x * xCof) == mapWidth - 1){
				if(map->GetCharacter(currentPoint) == LOT_DESIGNATOR){
					lotHit = true;
				}
				break;
			}
		}
		x--;
		LoggerWrapper::Debug(" CVH, east/west check complete");
		if(x < MIN_ROOM_SIZE_X){
			end = start;
			if(lotHit)
				return RoomCheckResult::NO_SPACE_LOT_0;
			else
				return RoomCheckResult::NO_SPACE_ROOM;
		}
		else{
			lotHit = false;
		}

		int y = 1;
		for (; y <= MAX_ROOM_SIZE_Y; y++) {
			Point curentPoint = Point(start.getX() + (x * xCof), start.getY() + (y * yCof));
			//Check southeast

			if (map->TileHasBeenSet(curentPoint) ||
					start.getY() + (y * yCof) == 0 ||
					start.getY() + (y * yCof) == mapHeight - 2){
				if(map->GetCharacter(curentPoint) == LOT_DESIGNATOR){
					lotHit = true;
				}
				break;
			}
		}
		y--;
		LoggerWrapper::Debug(" CVH, north/south check complete");
		if(y < MIN_ROOM_SIZE_X){
			end =  start;
			if(lotHit)
				return RoomCheckResult::NO_SPACE_LOT_0;
			else
				return RoomCheckResult::NO_SPACE_ROOM;
		}
		else{
			lotHit = false;
		}
		int tempY = 0;
		for(; tempY < y; tempY++ ){
			Point currentPoint = Point(start.getX() + (1 * xCof), start.getY() + (tempY * yCof));
			if (map->TileHasBeenSet(currentPoint)) {

				if(map->GetCharacter(currentPoint) == LOT_DESIGNATOR)
					lotHit = true;

				tempY--;
				if(tempY < MIN_ROOM_SIZE_X){
					end =  start;
					if(lotHit)
						return RoomCheckResult::NO_SPACE_LOT_0;
					else
						return RoomCheckResult::NO_SPACE_ROOM;
				}
				else{
					break;
				}
			}
		}
		y = tempY;
		LoggerWrapper::Debug(" CVH complete");
		end = Point(start.getX() + (x * xCof) , start.getY() + (y * yCof));
		return RoomCheckResult::SPACE;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::CheckHorizontalRoom");
		throw 0;
	}
}

bool Lot::InvalidRoomCorners(Point start, Point end) {
	try {
		bool result = start.getX() == end.getX() || start.getY() == end.getY() ||
				start.getX() == 0 || start.getY() == 0 ||
				end.getX() == 0 || end.getY() == 0 ||
				start.getX() >= mapWidth - 1 || start.getY() >= mapHeight - 2 ||
				end.getX() >= mapWidth - 1 || end.getY() >= mapHeight - 2;
		return result;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::InvalidRoomCorners");
		throw 0;
	}
}

void Lot::GenerateWindows(Room& room, MapGenerator::Orientation side) {
	try {
		switch(side){
		case MapGenerator::Orientation::NORTH:
			if(!room.GetWindowsNorth()){
				DrawNorthWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsNorth(true);
			}
			break;
		case MapGenerator::Orientation::SOUTH:
			if(!room.GetWindowsSouth()){
				DrawSouthWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsSouth(true);
			}
			break;
		case MapGenerator::Orientation::EAST:
			if(!room.GetWindowsEast()){
				DrawEastWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsEast(true);
			}
			break;
		case MapGenerator::Orientation::WEST:
			if(!room.GetWindowsWest()){
				DrawWestWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsWest(true);
			}
			break;
		default:
			break;
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::GenerateWindows");
		throw 0;
	}
}

void Lot::DrawNorthWindow(Point start, Point end) {
	try {
		bool validDoor = false;
		int x;
		do{
			x = randomWrap->getInt(start.getX() + 1, end.getX() - 1 );

			if(ValidDoor(x, start.getY())){
				validDoor = true;
			}

		} while(!validDoor);
		Point door(x, start.getY());

		MapGenerator::DrawHorizontalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawNorthWindow");
		throw 0;
	}
}

void Lot::DrawEastWindow(Point start, Point end) {
	try {
		int y;
		bool validDoor = false;
		do{
			y = randomWrap->getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(end.getX(), y)){
				validDoor = true;
			}
		} while(!validDoor);
		Point door(end.getX(), y );
		MapGenerator::DrawVerticalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawEastWindow");
		throw 0;
	}
}

void Lot::DrawSouthWindow(Point start, Point end) {
	try {
		int x;
		bool validDoor = false;
		do{
			x = randomWrap->getInt(start.getX() + 1, end.getX() - 1);
			if(ValidDoor(x, end.getY())){
				validDoor = true;
			}

		}while(!validDoor);

		Point door(x, end.getY());
		MapGenerator::DrawHorizontalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawSouthWindow");
		throw 0;
	}
}

void Lot::DrawWestWindow(Point start, Point end) {
	try {
		int y;
		bool validDoor = false;
		do{
			y = randomWrap->getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(start.getX(), y)){
				validDoor = true;
			}
		} while(!validDoor);
		Point door(start.getX(), y);
		MapGenerator::DrawVerticalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::DrawWestWindow");
		throw 0;
	}
}



Lot::RoomCheckResult Lot::CheckVerticalRoom(Point start, Point& end, bool xNegFlag, bool yNegFlag) {
	try {
		LoggerWrapper::Debug("CVR start: " + start.ToString());
		int xCof,yCof;

		bool lotHit = false;

		if(xNegFlag)
			xCof = -1;
		else
			xCof = 1;

		if(yNegFlag)
			yCof = -1;
		else
			yCof = 1;

		int y = 1;
		for (; y <= MAX_ROOM_SIZE_Y; y++) {
			Point currentPoint = Point(start.getX(), start.getY() + (y * yCof));
			//Check east
			if (map->TileHasBeenSet(currentPoint) ||
					start.getY() + (y * yCof) == 0 || start.getY() + (y * yCof) == mapHeight - 2){
				if(map->GetCharacter(currentPoint) == LOT_DESIGNATOR)
					lotHit = true;

				break;
			}
		}
		y--;
		LoggerWrapper::Debug("CVR, north/south check complete " + std::to_string(y));
		if(y < MIN_ROOM_SIZE_X){
			end = start;
			if(lotHit){
				if(y >= 1){
					return RoomCheckResult::NO_SPACE_LOT_1;
				}
				else{
					return RoomCheckResult::NO_SPACE_LOT_0;
				}

			}
			else{
				return RoomCheckResult::NO_SPACE_ROOM;
			}
		}
		else{
			lotHit = false;
		}


		int x = 1;
		for (; x <= MAX_ROOM_SIZE_X; x++) {
			Point currentPoint = Point(start.getX() + (x * xCof), start.getY() + (y * yCof));
			//Check southeast
			if (map->TileHasBeenSet(currentPoint) ||
					start.getX() + (x * xCof) == 0 || start.getX() + (x * xCof) == mapWidth - 1){
				if(map->GetCharacter(currentPoint) == LOT_DESIGNATOR){
					lotHit = true;
				}
				break;
			}
		}
		x--;

		if(x < MIN_ROOM_SIZE_X){
			end = start;
			if(lotHit){
				if(x >= 1){
					return RoomCheckResult::NO_SPACE_LOT_1;
				}
				else{
					return RoomCheckResult::NO_SPACE_LOT_0;
				}
			}
			else{
				return RoomCheckResult::NO_SPACE_ROOM;
			}
		}
		else{
			lotHit = false;
		}


		LoggerWrapper::Debug("CVR, east/west check complete " + std::to_string(x));
		int tempX = 0;
		for(; tempX < x; tempX++ ){
			Point currentPoint = Point(start.getX() + (tempX * xCof), start.getY() + (1 * yCof));
			if (map->TileHasBeenSet(currentPoint)) {
				tempX--;
				if(tempX < MIN_ROOM_SIZE_X){
					end = start;
					if(map->GetCharacter(currentPoint) == LOT_DESIGNATOR){
						if(tempX >= 1){
							return RoomCheckResult::NO_SPACE_LOT_1;
						}
						else{
							return RoomCheckResult::NO_SPACE_LOT_0;
						}
					}
					else{
						return RoomCheckResult::NO_SPACE_ROOM;
					}

				}
				else{
					break;
				}
			}
		}

		x = tempX;
		Point tempEnd = Point(start.getX() + (x * xCof), start.getY() + (y * yCof));
		LoggerWrapper::Debug(" CVR complete: " + tempEnd.ToString());
		end = tempEnd;
		return RoomCheckResult::SPACE;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Lot::CheckVerticalRoom");
		throw 0;
	}
}
