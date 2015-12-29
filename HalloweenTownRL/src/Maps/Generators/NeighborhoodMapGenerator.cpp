#include <algorithm>
#include <string>
#include <iostream>
#include <memory>
#include <set>

#include "libtcod.hpp"

#include "../Map.hpp"
#include "MapGenerator.hpp"
#include "NeighborhoodMapGenerator.hpp"
#include "../../Tile/TileColors.hpp"
#include "../../LoggerWrapper.hpp"

#ifndef NMG_LOGGER
#define NMG_LOGGER
#endif

NeighborhoodMapGenerator::NeighborhoodMapGenerator(int width, int height,
		MapGenerator::Orientation orientation): mapWidth(width), mapHeight(height), mapOri(orientation) {
	minRoomSizeX = 4;
	minRoomSizeY = 4;
	maxRoomSizeX = 7;
	maxRoomSizeY = 7;
	lotSizeX = 26;
	lotSizeY = 14;
	treeChance = 30;
	lotDesignator = TileCharacters::Default::DOUBLE_EXCLAMATION;
	furniture.push_back(TileCharacters::Default::BED);
	furniture.push_back(TileCharacters::Default::TV);
	furniture.push_back(TileCharacters::Default::TABLE);
	furniture.push_back(TileCharacters::Default::CHAIR);
	backDoor = false;
	neighborhoodMap = nullptr;
	map = nullptr;
}

void NeighborhoodMapGenerator::PopulateActors(Map* map) {
}


TCODMap* NeighborhoodMapGenerator::Generate(Map* map, bool generateActors) {
	try{
		//TODO this method is really ugly. Its the biggest obstacle to this being an easy to use class.
		int width = map->GetWidth();
		int height = map->GetHeight();
		neighborhoodMap = new TCODMap(width, height);
		this->map = map;
		TCODColor visible = TCODColor::grey;

		int lotX = 0;
		int lotY = 3;
		int yStreetSize = 7;
		CreateHouse(lotX, lotY, MapGenerator::Orientation::NORTH, visible);
		CreateHouse(lotX + lotSizeX, lotY, MapGenerator::Orientation::NORTH, visible);
		CreateHouse(lotX + lotSizeX * 2, lotY, MapGenerator::Orientation::NORTH, visible);
		CreateHouse(lotX, lotY + lotSizeY + yStreetSize, MapGenerator::Orientation::SOUTH, visible);
		CreateHouse(lotX + lotSizeX, lotY + lotSizeY + yStreetSize, MapGenerator::Orientation::SOUTH, visible);
		CreateHouse(lotX + lotSizeX * 2, lotY + lotSizeY + yStreetSize, MapGenerator::Orientation::SOUTH, visible);
		bool isTransparent = true;
		bool isWalkable = true;

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height-1; y++) {


				if(map->TileHasBeenSet(x, y)){
					if(map->GetCharacter(x, y) != TileCharacters::PERIOD){
						isTransparent = true;
						isWalkable = true;
						neighborhoodMap->setProperties(x, y, isTransparent, isWalkable);
						continue;
					}
					else{
						isTransparent = true;
						isWalkable = true;
						neighborhoodMap->setProperties(x, y, isTransparent, isWalkable);
					}
				}
				else{
					isTransparent = false;
					isWalkable = false;
					neighborhoodMap->setProperties(x, y, isTransparent, isWalkable);

					bool roadFlag = false;
					int lampPostRate = 17;
					switch(mapOri){
						case MapGenerator::Orientation::NORTH:
						case MapGenerator::Orientation::SOUTH:
							roadFlag = (x <= (width/2)+3  &&  x >= (width/2)-3);
							break;
						case MapGenerator::Orientation::EAST:
						case MapGenerator::Orientation::WEST:
							if(y == (height / 2 ) + 2  ||  y == (height / 2) - 3){
								if(y == (height / 2) + 2 && x % lampPostRate==0 && (x / lampPostRate) % 2 != 0 ) {
									map->actors.push(ActorFactory::CreateLampPost(x, y));
								}
								else if(y == (height / 2) - 3 && x % lampPostRate==0 && (x / lampPostRate) % 2 == 0 ){
									map->actors.push(ActorFactory::CreateLampPost(x, y));
								}
								else{
									DrawSidewalk(map, neighborhoodMap, x, y);
								}
							}
							else if(y <= (height/2)+2  &&  y >= (height/2)-3){
								DrawRoad(map, neighborhoodMap, x, y);
							}
							else{
								int tree = randomWrap.getInt(treeChance, 100);
								if(tree%100==0){
									DrawTree(map, neighborhoodMap, x, y);
								}
								else{
									DrawGrass(map, neighborhoodMap, x, y);
								}
							}
							break;
						default:
							LoggerWrapper::Error("Case " + std::to_string(mapOri) + " is not currently supported");
							break;
					}
				}
			}
		}
		return neighborhoodMap;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::Generate");
		throw 0;
	}
}


Room* NeighborhoodMapGenerator::FindNextDoor(Room room, Orientation potential) {
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
				NeighborhoodMapGenerator::RoomCheckResult result = CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == NeighborhoodMapGenerator::RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, Orientation::NORTH);
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

			Point tempEnd(0, 0);
			CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(end);
				xNeg = true;
				NeighborhoodMapGenerator::RoomCheckResult result = CheckVerticalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, Orientation::SOUTH);
					}
					return nullptr;
				}
			}

			int	x = std::abs((int)tempStart.getX() - (int)tempEnd.getX());
			int y = std::abs((int)tempStart.getY() - (int)tempEnd.getY());

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);
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
				NeighborhoodMapGenerator::RoomCheckResult result = CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, MapGenerator::Orientation::EAST);
					}
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
			Point tempEnd(0,0);
			CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);

			if(InvalidRoomCorners(tempStart, tempEnd)){
				tempStart = Point(start);
				yNeg = false;
				NeighborhoodMapGenerator::RoomCheckResult result = CheckHorizontalRoom(tempStart, tempEnd, xNeg, yNeg);
				if(InvalidRoomCorners(tempStart, tempEnd)){
					if(result == RoomCheckResult::NO_SPACE_LOT_0 || result == RoomCheckResult::NO_SPACE_LOT_1){
						GenerateWindows(room, MapGenerator::Orientation::WEST);
					}
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
		map->actors.push(ActorFactory::CreateDoor(door.getX(), door.getY()));

	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawNorthDoor(Point start, Point end) {
	try {
		bool validDoor = false;
		int x;
		do{
			x = randomWrap.getInt(start.getX() + 1, end.getX() - 1 );

			if(ValidDoor(x, start.getY())){
				validDoor = true;
			}

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
		bool validDoor = false;
		do{
			y = randomWrap.getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(end.getX(), y)){
				validDoor = true;
			}
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
		bool validDoor = false;
		do{
			y = randomWrap.getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(start.getX(), y)){
				validDoor = true;
			}
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
		bool validDoor = false;
		do{
			x = randomWrap.getInt(start.getX() + 1, end.getX() - 1);
			if(ValidDoor(x, end.getY())){
				validDoor = true;
			}

		}while(!validDoor);

		Point door(x, end.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSouthDoor");
		throw 0;
	}
}



void NeighborhoodMapGenerator::DrawNorthWindow(Point start, Point end) {
	try {
		bool validDoor = false;
		int x;
		do{
			x = randomWrap.getInt(start.getX() + 1, end.getX() - 1 );

			if(ValidDoor(x, start.getY())){
				validDoor = true;
			}

		} while(!validDoor);
		Point door(x, start.getY());

		DrawHorizontalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawEastWindow(Point start, Point end) {
	try {
		int y;
		bool validDoor = false;
		do{
			y = randomWrap.getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(end.getX(), y)){
				validDoor = true;
			}
		} while(!validDoor);
		Point door(end.getX(), y );
		DrawVerticalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawEastWindow");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawWestWindow(Point start, Point end) {
	try {
		int y;
		bool validDoor = false;
		do{
			y = randomWrap.getInt(start.getY() + 1, end.getY() - 1);
			if(ValidDoor(start.getX(), y)){
				validDoor = true;
			}
		} while(!validDoor);
		Point door(start.getX(), y);
		DrawVerticalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawWestWindow");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawSouthWindow(Point start, Point end) {
	try {
		int x;
		bool validDoor = false;
		do{
			x = randomWrap.getInt(start.getX() + 1, end.getX() - 1);
			if(ValidDoor(x, end.getY())){
				validDoor = true;
			}

		}while(!validDoor);

		Point door(x, end.getY());
		DrawHorizontalWindow(map, neighborhoodMap, door.getX(), door.getY());
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSouthWindow");
		throw 0;
	}
}

Room* NeighborhoodMapGenerator::FindNextDoor(Room room) {
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::FindNextDoor");
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

				int tempChar = randomWrap.getInt(i + j, end.getX() + end.getY());
				if(map->GetCharacter(i - 1, j) != TileCharacters::Default::PLUS &&
						map->GetCharacter(i + 1, j) != TileCharacters::Default::PLUS &&
						map->GetCharacter(i, j + 1) != TileCharacters::Default::PLUS &&
						map->GetCharacter(i, j - 1) != TileCharacters::Default::PLUS &&
						furnitureRoom != 0 && tempChar == end.getX() + end.getY()){
					furnitureRoom--;
					character = furniture.at(randomWrap.getInt(0, 3));
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::GenerateInterior");
		throw 0;
	}
}


int NeighborhoodMapGenerator::GenerateRoom(Room room, TCODColor color, int roomsLeft) {
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::GenerateRoom");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawNextDoor(Room* ra) {
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNextDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::CreateHouse(int lotX, int lotY, MapGenerator::Orientation side, TCODColor visible) {
	try {
#ifdef NMG_LOGGER
		LoggerWrapper::Debug("Creating House");
#endif
		Point lotStart(lotX, lotY);
		Point lotEnd(lotX + lotSizeX, lotY + lotSizeY);
		backDoor = false;
		Point fenceStart(0,0);
		Point fenceEnd(0,0);
		DrawRectangle(map, lotStart, lotEnd, TileColors::white, lotDesignator);

		int xSectionCount = lotSizeX / (minRoomSizeX); //Break into sections based off the minimum room size
		int xSection = randomWrap.getInt(0, xSectionCount - 1); //Randomly selects which X section will be used
		int xOffset = minRoomSizeX * xSection;
		xOffset = std::max(1, xOffset); //Calculates the offset and makes sure that it is at least 1

		int ySectionCount = lotSizeY / (minRoomSizeY);
		int ySection;
		switch(side){
		case Orientation::SOUTH: //South side of the road
			ySection = 0;
			fenceStart = Point(lotStart, 0, lotSizeY/2);
			fenceEnd = Point(lotEnd, 0, 3);
			break;
		case Orientation::NORTH: //North side of the road
			ySection = ySectionCount - 1;
			fenceStart = Point(lotStart, 0, -3);
			fenceEnd = Point(lotEnd, 0, -lotSizeY/2);
			break;
		default:
			LoggerWrapper::Error("An incorrect orientation was used in CreateHouse " + std::to_string(mapOri));
			throw 0;
			break;

		}
		int yOffset = minRoomSizeY * ySection;
		yOffset = std::max(1, yOffset);

		Point buildingStart(lotX + xOffset, lotY + yOffset);
		LoggerWrapper::Debug("StartX: " + std::to_string(buildingStart.getX()) + " StartY: " + std::to_string(buildingStart.getY()));

		int roomsLeft = 6;

		//lot start + lot size (aka end of lot)
		int maxXSize = std::min( (lotX + lotSizeX) - (lotX + xOffset), maxRoomSizeX);
		int maxYSize = std::min( (lotY + lotSizeY) - (lotY + yOffset), maxRoomSizeY);
		Point buildingEnd(lotX, lotY);
		do{
			int sizeX = randomWrap.getInt(minRoomSizeX, maxXSize);
			int sizeY = randomWrap.getInt(minRoomSizeY, maxYSize);
			buildingEnd = Point(buildingStart, sizeX, sizeY);
		}while(map->TileHasBeenSet(buildingEnd));

		Room initialRoom(buildingStart, buildingEnd, side);
		GenerateRoom(initialRoom, visible, roomsLeft);
		DrawRectangle(map, lotStart, lotEnd, TileColors::white, TileCharacters::Default::RAINBOW);
		DrawRectangle(map, fenceStart, fenceEnd, TileColors::brownLight, TileCharacters::Default::HASH, true);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CreateHouse");
		throw 0;
	}
}

NeighborhoodMapGenerator::RoomCheckResult NeighborhoodMapGenerator::CheckHorizontalRoom(Point start, Point &end, bool xNegFlag, bool yNegFlag) {
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
		for (; x <= maxRoomSizeX; x++) {
			Point currentPoint = Point(start.getX() + (x * xCof), start.getY());
			//Check east
			if (map->TileHasBeenSet(currentPoint) ||
					start.getX() + (x * xCof) == 0 ||
					start.getX() + (x * xCof) == mapWidth - 1){
				if(map->GetCharacter(currentPoint) == lotDesignator){
					lotHit = true;
				}
				break;
			}
		}
		x--;
		LoggerWrapper::Debug(" CVH, east/west check complete");
		if(x < minRoomSizeX){
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
		for (; y <= maxRoomSizeY; y++) {
			Point curentPoint = Point(start.getX() + (x * xCof), start.getY() + (y * yCof));
			//Check southeast

			if (map->TileHasBeenSet(curentPoint) ||
					start.getY() + (y * yCof) == 0 ||
					start.getY() + (y * yCof) == mapHeight - 2){
				if(map->GetCharacter(curentPoint) == lotDesignator){
					lotHit = true;
				}
				break;
			}
		}
		y--;
		LoggerWrapper::Debug(" CVH, north/south check complete");
		if(y < minRoomSizeY){
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

				if(map->GetCharacter(currentPoint) == lotDesignator)
					lotHit = true;

				tempY--;
				if(tempY < minRoomSizeY){
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CheckHorizontalRoom");
		throw 0;
	}
}

bool NeighborhoodMapGenerator::ValidDoor(const int x, const int y) {
	try {
		bool validDoor = false;
		LoggerWrapper::Debug("Door X:" + std::to_string(x) + " Y:" + std::to_string(y));
		int character =  map->GetCharacter(x, y);
		int invalidCharacters[] = {TileCharacters::Default::DOUBLE_PIPE_T_BOTTOM, TileCharacters::Default::DOUBLE_PIPE_T_LEFT, TileCharacters::Default::DOUBLE_PIPE_T_RIGHT, TileCharacters::Default::DOUBLE_PIPE_T_TOP };
		validDoor = (std::find(std::begin(invalidCharacters), std::end(invalidCharacters), character) == std::end(invalidCharacters));

		LoggerWrapper::Debug("Character of door tile " + std::to_string(character) + " is valid " + std::to_string(validDoor));
		return validDoor;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::ValidDoor");
		throw 0;
	}
}


NeighborhoodMapGenerator::RoomCheckResult NeighborhoodMapGenerator::CheckVerticalRoom(Point start, Point &end, bool xNegFlag, bool yNegFlag) {
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
		for (; y <= maxRoomSizeY; y++) {
			Point currentPoint = Point(start.getX(), start.getY() + (y * yCof));
			//Check east
			if (map->TileHasBeenSet(currentPoint) ||
					start.getY() + (y * yCof) == 0 || start.getY() + (y * yCof) == mapHeight - 2){
				if(map->GetCharacter(currentPoint) == lotDesignator)
					lotHit = true;

				break;
			}
		}
		y--;
		LoggerWrapper::Debug("CVR, north/south check complete " + std::to_string(y));
		if(y < minRoomSizeY){
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
		for (; x <= maxRoomSizeX; x++) {
			Point currentPoint = Point(start.getX() + (x * xCof), start.getY() + (y * yCof));
			//Check southeast
			if (map->TileHasBeenSet(currentPoint) ||
					start.getX() + (x * xCof) == 0 || start.getX() + (x * xCof) == mapWidth - 1){
				if(map->GetCharacter(currentPoint) == lotDesignator){
					lotHit = true;
				}
				break;
			}
		}
		x--;

		if(x < minRoomSizeX){
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
				if(tempX < minRoomSizeX){
					end = start;
					if(map->GetCharacter(currentPoint) == lotDesignator){
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CheckVerticalRoom");
		throw 0;
	}

}

bool NeighborhoodMapGenerator::InvalidRoomCorners(Point start, Point end) {
	try {
		bool result = start.getX() == end.getX() || start.getY() == end.getY() ||
				start.getX() == 0 || start.getY() == 0 ||
				end.getX() == 0 || end.getY() == 0 ||
				start.getX() >= mapWidth - 1 || start.getY() >= mapHeight - 2 ||
				end.getX() >= mapWidth - 1 || end.getY() >= mapHeight - 2;
		return result;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::InvalidRoomCorners");
		throw 0;
	}
}

void NeighborhoodMapGenerator::GenerateWindows(Room& room, MapGenerator::Orientation side) {
	try {
		switch(side){
		case NORTH:
			if(!room.GetWindowsNorth()){
				DrawNorthWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsNorth(true);
			}
			break;
		case SOUTH:
			if(!room.GetWindowsSouth()){
				DrawSouthWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsSouth(true);
			}
			break;
		case EAST:
			if(!room.GetWindowsEast()){
				DrawEastWindow(room.getNWCorner(), room.getSECorner());
				room.SetWindowsEast(true);
			}
			break;
		case WEST:
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::GenerateWindows");
		throw 0;
	}
}
