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
	minRoomSizeX = 5;
	minRoomSizeY = 5;
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
						neighborhoodMap->setProperties(x, y, false, false);
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
		map->actors.push(ActorFactory::CreateLampPost(65, 7));

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


//TODO There are still cases where a room will overwrite another room.
Room* NeighborhoodMapGenerator::FindNextDoor(Point start, Point end, Orientation potential) {
	try {
		switch(potential){
		case MapGenerator::Orientation::NORTH:{
			int y = 1; //TODO Clean this function up. Its really ugly.
		//I think you could break each of these case statements out into their own methods. Those methods might be broken into generate offset
		// and generate points/room. Point generation is done a case by case basis so I don't think that is something that can be made generic.
			for(; y < maxRoomSizeY && y < start.getY(); y++){  //Check north
				if(map->TileHasBeenSet(Point(end.getX() , start.getY() - y))){
					break;
				}
			}
			y--;

			if(y < minRoomSizeY)
				return nullptr;

			int x = 1;
			for(; x <= maxRoomSizeX && x < start.getX(); x++){
				if(map->TileHasBeenSet(Point(end.getX() - x, start.getY() - y))){
					break;
				}
			}

			x--;
			if(x < minRoomSizeX)
				return nullptr;

			int temp = y;
			for(; temp > 1; temp--){
				if(map->TileHasBeenSet(Point(end.getX() - x, start.getY() - temp))){
					return nullptr;
				}
			}

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);

			Point newEnd(end.getX(), start.getY());
			Point newStart(newEnd.getX() - offsetX, newEnd.getY() - offsetY);
			Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::NORTH);
			return r;

		}
		break;
		case MapGenerator::Orientation::SOUTH:{
			int y = 1;
			for(; y < maxRoomSizeY; y++){  //Check south
				if(map->TileHasBeenSet(Point(start.getX() , end.getY() + y))){
					break;
				}
			}
			y--;

			if(y < minRoomSizeY)
				return nullptr;

			int x = 1;
			for(; x <= maxRoomSizeX; x++){
				if(map->TileHasBeenSet(Point(start.getX() + x, end.getY() + y))){
					break;
				}
			}

			x--;
			if(x < minRoomSizeX)
				return nullptr;

			int temp = y;
			for(; temp > 1; temp--){
				if(map->TileHasBeenSet(Point(start.getX() + x, end.getY() + temp))){
					return nullptr;
				}
			}

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);

			Point newStart(start.getX(), end.getY());
			Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);


			Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::SOUTH);

			return r;

		}
		break;
		case MapGenerator::Orientation::EAST:{
			int x = 1;
			for(; x <= maxRoomSizeX; x++){ //Check east
				if(map->TileHasBeenSet(Point(end.getX() + x, start.getY()))){
					break;
				}
			}
			x--;
			if(x < minRoomSizeX)
				return nullptr;

			int y = 1;
			for(; y < maxRoomSizeY; y++){  //Check southeast
				if(map->TileHasBeenSet(Point(end.getX() + x , start.getY() + y))){
					break;
				}
			}
			y--;

			if(y < minRoomSizeY)
				return nullptr;

			int temp = x;
			for(; temp > 1; temp--){  //Check southeast
				if(map->TileHasBeenSet(Point(end.getX() + temp , start.getY() + y))){
					return nullptr;
				}
			}

			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);

			Point newStart(end.getX(), start.getY());
			Point newEnd(newStart.getX() + offsetX, newStart.getY() + offsetY);

			Room* r = new Room(newStart, newEnd, MapGenerator::Orientation::EAST);

			return r;

		}
		break;
		case MapGenerator::Orientation::WEST:{
			int x = 1;
			for(; x <= maxRoomSizeX && x < start.getX(); x++){
				if(map->TileHasBeenSet(Point(start.getX() - x, end.getY()))){
					break;
				}
			}

			x--;
			if(x < minRoomSizeX)
				return nullptr;

			int y = 1;
			for(; y < maxRoomSizeY && y < start.getY(); y++){  //Check west
				if(map->TileHasBeenSet(Point(start.getX() - x, end.getY() - y))){
					break;
				}
			}
			y--;

			if(y < minRoomSizeY)
				return nullptr;


			int temp = x;
			for(; temp > 1; temp--){  //Check west
				if(map->TileHasBeenSet(Point(start.getX() - temp, end.getY() - y))){
					return nullptr;
				}
			}




			int offsetX = randomWrap.getInt(minRoomSizeX, x);
			int offsetY = randomWrap.getInt(minRoomSizeY, y);
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
		Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1 ), start.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNorthDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawEastDoor(Point start, Point end) {
	try {
		Point door(end.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawEastDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawSouthDoor(Point start, Point end) {
	try {
		Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1), end.getY());
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSouthDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawWestDoor(Point start, Point end) {
	try {
		Point door(start.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
		DrawDoor(door);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawWestDoor");
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
				int character = TileCharacters::Default::PERIOD;
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

					DrawNextDoor(ra);

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
			LoggerWrapper::Info( "Next door is south" );
			#endif
			DrawSouthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::EAST:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is west");
			#endif
			DrawWestDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::SOUTH:
			#ifdef NMG_LOGGER
			LoggerWrapper::Info("Next door is north");
			#endif
			DrawNorthDoor(ra->getNWCorner(), ra->getSECorner());
			break;
		case MapGenerator::Orientation::WEST:
			#ifdef NMG_LOGGER
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
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawNextDoor");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawWalls(Orientation previousOrientation, const Room& room,
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
		EraseFence(lotStart, lotEnd);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::CreateHouse");
		throw 0;
	}
}
