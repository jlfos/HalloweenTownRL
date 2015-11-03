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

TCODMap* RoadMapGenerator::Generate(Map* map, bool generateActors){
	try{
		int width = map->GetWidth();
		int height = map->GetHeight();
		roadMap = new TCODMap(width, height);
		this->map = map;
		TCODColor visible = TCODColor::grey;
		int sizeX = randomWrap.getInt(minSizeX, maxSizeX, false);
		int sizeY = randomWrap.getInt(minSizeY, maxSizeY, false);
		Point lotStart(10, 10);
		Point lotEnd(30, 30);
		DrawFence(lotStart, lotEnd);
		Point buildingStart(20, 20);
		MapGenerator::Orientation side = randomWrap.GetOrientation();
		int roomsLeft = 6;
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
	//TODO we need to work on back up. If ra comes back as null then you need to back up to the previous level and try again.
	switch (ra->getOrientation()) {
	case MapGenerator::Orientation::NORTH:
		std::cout << "Next door is south" << std::endl;
		DrawSouthDoor(ra->getStart(), ra->getEnd());
		break;
	case MapGenerator::Orientation::EAST:
		std::cout << "Next door is west" << std::endl;
		DrawWestDoor(ra->getStart(), ra->getEnd());
		break;
	case MapGenerator::Orientation::SOUTH:
		std::cout << "Next door is north" << std::endl;
		DrawNorthDoor(ra->getStart(), ra->getEnd());
		break;
	case MapGenerator::Orientation::WEST:
		std::cout << "Next door is east" << std::endl;
		DrawEastDoor(ra->getStart(), ra->getEnd());
		break;
	default:
		std::cerr << "The value " << ra->getOrientation() << " is not currently supported"
				<< std::endl;
		throw 0;
	}
}

void RoadMapGenerator::DrawWalls(Orientation previousOrientation, const Room& room,
		TCODColor color) {
	/**TODO
	 * You also might need to pass in the size of the room as a parameter.
	 */
	if (previousOrientation != MapGenerator::Orientation::SOUTH) {
		DrawNorthWall(room.getStart(), room.getEnd(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawSouthDoor(room.getStart(), room.getEnd());
	}
	if (previousOrientation != MapGenerator::Orientation::NORTH) {
		DrawSouthWall(room.getStart(), room.getEnd(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawNorthDoor(room.getStart(), room.getEnd());
	}
	if (previousOrientation != MapGenerator::Orientation::WEST) {
		DrawEastWall(room.getStart(), room.getEnd(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawWestDoor(room.getStart(), room.getEnd());
	}
	if (previousOrientation != MapGenerator::Orientation::EAST) {
		DrawWestWall(room.getStart(), room.getEnd(), color);
		if (previousOrientation == MapGenerator::Orientation::NONE)
			DrawEastDoor(room.getStart(), room.getEnd());
	}
}

int RoadMapGenerator::GenerateRoom(Room room, TCODColor color, Orientation previousOrientation, int roomsLeft) {
	try{
		//TODO You also might need to pass in the size of the room as a parameter.

		DrawWalls(previousOrientation, room, color);

		DrawInterior(room.getStart(), room.getEnd(), roomsLeft);

		roomsLeft--;
		std::cout << "Current Start X: " << room.getStart().getX() << "Current Start Y: " << room.getStart().getY() << std::endl;
		std::cout << "Current End X: " << room.getEnd().getX() << "Current End Y: " << room.getEnd().getY() << std::endl;
		if(roomsLeft == 0)
			return 0;
		else{
			Room* ra;
			do{
				Point start = room.getStart();
				Point end = room.getEnd();

				ra = FindNextDoor(start, end);
				if(ra != nullptr){
					//TODO we need to work on back up. If ra comes back as null then you need to back up to the previous level and try again.
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
	int character = TileCharacters::Default::RAINBOW;
	DrawNWCorner(start, color);
	for(int i = start.getX() + 1; i < end.getX(); i++){
		character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
		map->SetTileProperties(Point(i, start.getY()), color, character);
	}
	DrawNECorner(Point(end.getX(), start.getY()), color);
}

void RoadMapGenerator::DrawSouthWall(Point start, Point end, TCODColor color) {
		int character = TileCharacters::Default::RAINBOW;
		DrawSWCorner(Point(start.getX(), end.getY()), color);
		for(int i = start.getX() + 1; i < end.getX(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_HORIZONTAL;
			map->SetTileProperties(Point(i, end.getY()), color, character);
		}
		DrawSECorner(end, color);
}

void RoadMapGenerator::DrawEastWall(Point start, Point end, TCODColor color) {
		int character = TileCharacters::Default::RAINBOW;
		DrawNECorner(Point(end.getX(), start.getY()), color);
		for(int i = start.getY() + 1; i < end.getY(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			map->SetTileProperties(Point(end.getX(), i), color, character);
		}
		DrawSECorner(end, color);
}

void RoadMapGenerator::DrawWestWall(Point start, Point end, TCODColor color) {
		int character = TileCharacters::Default::RAINBOW;
		DrawNWCorner(start, color);

		for(int i = start.getY() + 1; i < end.getY(); i++){
			character = TileCharacters::Default::DOUBLE_PIPE_VERTICAL;
			map->SetTileProperties(Point(start.getX(), i), color, character);
		}
		DrawSWCorner(Point(start.getX(), end.getY()), color);
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
	DrawDoor(door);
}

void RoadMapGenerator::DrawSouthDoor(Point start, Point end) {
	Point door(randomWrap.getInt(start.getX() + 1, end.getX() - 1), end.getY());
	DrawDoor(door);
}

void RoadMapGenerator::DrawWestDoor(Point start, Point end) {
	Point door(start.getX(), randomWrap.getInt(start.getY() + 1, end.getY() - 1));
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
	//TODO the switching of the directions needs to be tested
	Room* r = nullptr;
	Orientation side;// =  (Orientation)randomWrap.getInt(0, 3);//randomWrap.GetOrientation();
	std::set<MapGenerator::Orientation> orientationSet = {MapGenerator::Orientation::NORTH,
														  MapGenerator::Orientation::EAST,
														  MapGenerator::Orientation::WEST,
														  MapGenerator::Orientation::SOUTH};
	while(notFound){ //TODO this has the potential to go on forever, this bug needs to be solved now
		side = randomWrap.GetOrientation();
		auto search = orientationSet.find(side);
		if(search != orientationSet.end()){
			orientationSet.erase(search);
//			std::cout << "ori rand " << side << std::endl;
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
	DrawNorthWall(start, end, TCODColor::white);
	DrawSouthWall(start, end, TCODColor::white);
	DrawEastWall(start, end, TCODColor::white);
	DrawWestWall(start, end, TCODColor::white);
}

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

		int offsetX = randomWrap.getInt(minSizeX, x, true);
		int offsetY = randomWrap.getInt(minSizeY, y, true);

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

		int offsetX = randomWrap.getInt(minSizeX, x, true);
		int offsetY = randomWrap.getInt(minSizeY, y, true);

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

		int offsetX = randomWrap.getInt(minSizeX, x, true);
		int offsetY = randomWrap.getInt(minSizeY, y, true);

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

		int offsetX = randomWrap.getInt(minSizeX, x, true);
		int offsetY = randomWrap.getInt(minSizeY, y, true);
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
