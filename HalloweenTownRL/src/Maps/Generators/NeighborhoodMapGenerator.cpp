#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <string>

#include "libtcod.hpp"

#include "../Lot.hpp"
#include "../Map.hpp"
#include "MapGenerator.hpp"
#include "NeighborhoodMapGenerator.hpp"
#include "../../Tile/TileColors.hpp"
#include "../../LoggerWrapper.hpp"

#ifndef NMG_LOGGER
#define NMG_LOGGER
#endif

//TODO move these into their own functions
auto xWestVerticalRoad = [](int width){return (width / 2) - 2;};
auto xEastVerticalRoad = [](int width ) {return (width / 2) + 1;};
auto ySouthHorizontalRoad = [](int height) {return (height / 2) + 1;};
auto yNorthHorizontalRoad = [](int height) {return (height / 2) - 2;};


NeighborhoodMapGenerator::NeighborhoodMapGenerator(int width, int height,
		MapGenerator::Orientation orientation, NeighborhoodDetails details): mapWidth(width), mapHeight(height), mapOri(orientation) {
	minRoomSizeX = 4;
	minRoomSizeY = 4;
	maxRoomSizeX = 7;
	maxRoomSizeY = 7;
	lotSizeX = 26;
	lotSizeY = 14;
	lampPostRate = 17;
	treeChance = 30;
	lotDesignator = TileCharacters::Default::DOUBLE_EXCLAMATION;
	furniture.push_back(TileCharacters::Default::BED);
	furniture.push_back(TileCharacters::Default::TV);
	furniture.push_back(TileCharacters::Default::TABLE);
	furniture.push_back(TileCharacters::Default::CHAIR);
	backDoor = false;
	neighborhoodMap = nullptr;
	map = nullptr;
	this->details = details;
}

NeighborhoodMapGenerator::~NeighborhoodMapGenerator() {
	delete neighborhoodMap;
}

void NeighborhoodMapGenerator::PopulateActors(Map* map) {
}


TCODMap* NeighborhoodMapGenerator::Generate(Map* map, bool generateActors) {
	try{
		//TODO this method is really ugly. Its the biggest obstacle to this being an easy to use class.
		neighborhoodMap = new TCODMap(mapWidth, mapHeight);
		this->map = map;


		bool isTransparent = true;
		bool isWalkable = true;

		DrawRoads();
		DrawSidewalks();
		DrawLots();

		for (int x = 0; x < mapWidth; x++) {
			for (int y = 0; y < mapHeight-1; y++) {
				if(map->TileHasBeenSet(x, y)){
					if(map->GetCharacter(x, y) != TileCharacters::PERIOD){ //Set not period
						isTransparent = true;
						isWalkable = true;
						neighborhoodMap->setProperties(x, y, isTransparent, isWalkable);
						continue;
					}
					else{ //Set period
						isTransparent = true;
						isWalkable = true;
						neighborhoodMap->setProperties(x, y, isTransparent, isWalkable);
					}
				}
				else{ //Not set
					isTransparent = true;
					isWalkable = true;
					neighborhoodMap->setProperties(x, y, isTransparent, isWalkable);
					int tree = randomWrap.getInt(treeChance, 100);
					if(tree%100==0){
						DrawTree(map, neighborhoodMap, x, y);
					}
					else{
						DrawGrass(map, neighborhoodMap, x, y);
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




void NeighborhoodMapGenerator::DrawVerticalSidewalk(Point start, Point end, bool evenLampposts) {
	try {
		if(start.getX() != end.getX()){
			LoggerWrapper::Error("Start and End points must be horizontally aligned.");
			throw 0;
		}

		if(start.getY() > end.getY()){
			LoggerWrapper::Error("Start point must come before End point.");
			throw 0;
		}

		for(u_int y = start.getY(); y <= end.getY(); y++){
			if(evenLampposts){
				if(y % lampPostRate== 0 && (y / lampPostRate) % 2 == 0)
					map->actors.push(ActorFactory::CreateLampPost(start.getX(), y));
				else
					DrawSidewalk(map, neighborhoodMap, start.getX(), y);
			}
			else{
				if (y % lampPostRate == 0 && (y / lampPostRate) % 2 != 0)
					map->actors.push(ActorFactory::CreateLampPost(start.getX(), y));
				else
					DrawSidewalk(map, neighborhoodMap, start.getX(), y);
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawVerticalSidewalk");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawHorizontalSidewalk(Point start, Point end, bool evenLampposts) {
	try {
		if(start.getY() != end.getY()){
			LoggerWrapper::Error("Start and End points must be vertically aligned.");
			throw 0;
		}

		if(start.getX() > end.getX()){
			LoggerWrapper::Error("Start point must come before End point.");
			throw 0;
		}

		for(u_int x = start.getX(); x <= end.getX(); x++){
			if(evenLampposts){
				if(x % lampPostRate== 0 && (x / lampPostRate) % 2 == 0)
					map->actors.push(ActorFactory::CreateLampPost(x, start.getY()));
				else
					DrawSidewalk(map, neighborhoodMap, x, start.getY());
			}
			else{
				if (x % lampPostRate == 0 && (x / lampPostRate) % 2 != 0)
					map->actors.push(ActorFactory::CreateLampPost(x, start.getY()));
				else
					DrawSidewalk(map, neighborhoodMap, x, start.getY());
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawHorizontalSidewalk");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawRoads() {
	try {
		if(details.eastRoad){
			Point roadStart(mapWidth / 2, yNorthHorizontalRoad(mapHeight));
			Point roadEnd(mapWidth - 1, ySouthHorizontalRoad(mapHeight));
			DrawFilledArea(map, roadStart, roadEnd, TileColors::grey, TileCharacters::Default::PERIOD);
		}
		if(details.westRoad){
			Point roadStart = Point(0, yNorthHorizontalRoad(mapHeight));
			Point roadEnd = Point(mapWidth / 2, ySouthHorizontalRoad(mapHeight));
			DrawFilledArea(map, roadStart, roadEnd, TileColors::grey, TileCharacters::Default::PERIOD);
		}
		if(details.northRoad){
			Point roadStart = Point( xWestVerticalRoad(mapWidth), 0);
			Point roadEnd = Point( xEastVerticalRoad(mapWidth), ySouthHorizontalRoad(mapHeight));
			DrawFilledArea(map, roadStart, roadEnd, TileColors::grey, TileCharacters::Default::PERIOD);
		}
		if(details.southRoad){
			Point roadStart = Point( xWestVerticalRoad(mapWidth), yNorthHorizontalRoad(mapHeight));
			Point roadEnd = Point( xEastVerticalRoad(mapWidth), mapHeight - 1);
			DrawFilledArea(map, roadStart, roadEnd, TileColors::grey, TileCharacters::Default::PERIOD);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawRoads");
		throw 0;
	}

}

void NeighborhoodMapGenerator::DrawSidewalks() {
	try {
		if(details.eastRoad){
			Point sideWalkStart(xEastVerticalRoad(mapWidth) + 1, yNorthHorizontalRoad(mapHeight) - 1);
			Point sideWalkEnd( mapWidth - 1, yNorthHorizontalRoad(mapHeight) - 1);
			DrawHorizontalSidewalk(sideWalkStart, sideWalkEnd, true);

			sideWalkStart = Point(xEastVerticalRoad(mapWidth) + 1, ySouthHorizontalRoad(mapHeight) + 1);
			sideWalkEnd = Point( mapWidth - 1, ySouthHorizontalRoad(mapHeight) + 1);
			DrawHorizontalSidewalk(sideWalkStart, sideWalkEnd, false);
		}
		else{
			Point sideWalkStart(xEastVerticalRoad(mapWidth) + 1, yNorthHorizontalRoad(mapHeight) - 1);
			Point sideWalkEnd( xEastVerticalRoad(mapWidth) + 1, ySouthHorizontalRoad(mapHeight) + 1);
			DrawVerticalSidewalk(sideWalkStart, sideWalkEnd, true);
		}

		if(details.westRoad){
			Point sideWalkStart(0, yNorthHorizontalRoad(mapHeight) - 1);
			Point sideWalkEnd( xWestVerticalRoad(mapWidth) - 1, yNorthHorizontalRoad(mapHeight) - 1);
			DrawHorizontalSidewalk(sideWalkStart, sideWalkEnd, true);

			sideWalkStart = Point(0, ySouthHorizontalRoad(mapHeight) + 1);
			sideWalkEnd = Point(  xWestVerticalRoad(mapWidth) - 1, ySouthHorizontalRoad(mapHeight) + 1);
			DrawHorizontalSidewalk(sideWalkStart, sideWalkEnd, false);
		}
		else{
			Point sideWalkStart(xWestVerticalRoad(mapWidth) - 1, yNorthHorizontalRoad(mapHeight) - 1);
			Point sideWalkEnd( xWestVerticalRoad(mapWidth) - 1, ySouthHorizontalRoad(mapHeight) + 1);
			DrawVerticalSidewalk(sideWalkStart, sideWalkEnd, true);
		}

		if(details.northRoad){
			Point sideWalkStart(xWestVerticalRoad(mapWidth) - 1, 0);
			Point sideWalkEnd( xWestVerticalRoad(mapWidth) - 1, yNorthHorizontalRoad(mapHeight) - 1 );
			DrawVerticalSidewalk(sideWalkStart, sideWalkEnd, true);

			sideWalkStart = Point(xEastVerticalRoad(mapWidth) + 1, 0);
			sideWalkEnd = Point(  xEastVerticalRoad(mapWidth) + 1, yNorthHorizontalRoad(mapHeight) - 1);
			DrawVerticalSidewalk(sideWalkStart, sideWalkEnd, false);
		}
		else{
			Point sideWalkStart(xWestVerticalRoad(mapWidth), yNorthHorizontalRoad(mapHeight) - 1);
			Point sideWalkEnd( xEastVerticalRoad(mapWidth), yNorthHorizontalRoad(mapHeight) - 1);
			DrawHorizontalSidewalk(sideWalkStart, sideWalkEnd, true);
		}


		if(details.southRoad){
			Point sideWalkStart(xWestVerticalRoad(mapWidth) - 1, ySouthHorizontalRoad(mapHeight) + 1);
			Point sideWalkEnd( xWestVerticalRoad(mapWidth) - 1, mapHeight - 1);
			DrawVerticalSidewalk(sideWalkStart, sideWalkEnd, true);

			sideWalkStart = Point(xEastVerticalRoad(mapWidth) + 1, ySouthHorizontalRoad(mapHeight) + 1);
			sideWalkEnd = Point(  xEastVerticalRoad(mapWidth) + 1, mapHeight - 1);
			DrawVerticalSidewalk(sideWalkStart, sideWalkEnd, false);
		}
		else{
			Point sideWalkStart(xWestVerticalRoad(mapWidth), ySouthHorizontalRoad(mapHeight) + 1);
			Point sideWalkEnd(xEastVerticalRoad(mapWidth), ySouthHorizontalRoad(mapHeight) + 1);
			DrawHorizontalSidewalk(sideWalkStart, sideWalkEnd, false);
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in NeighborhoodMapGenerator::DrawSidewalks");
		throw 0;
	}
}

void NeighborhoodMapGenerator::DrawLots() {
	if(details.eastRoad){
		Lot ne(neighborhoodMap, map, &randomWrap, Lot::LotPosition::NORTHEAST, Lot::LotOrientation::NS);
		ne.PopulateLot();

		Lot se(neighborhoodMap, map, &randomWrap, Lot::LotPosition::SOUTHEAST, Lot::LotOrientation::NS);
		se.PopulateLot();
	}
	if(!details.eastRoad && !details.southRoad){
		Lot se(neighborhoodMap, map, &randomWrap, Lot::LotPosition::SOUTHEAST, Lot::LotOrientation::EMPTY);
		se.PopulateLot();
	}


	if(details.westRoad){
		Lot nw(neighborhoodMap, map, &randomWrap, Lot::LotPosition::NORTHWEST, Lot::LotOrientation::NS);
		nw.PopulateLot();

		Lot sw(neighborhoodMap, map, &randomWrap, Lot::LotPosition::SOUTHWEST, Lot::LotOrientation::NS);
		sw.PopulateLot();
	}
	else{

	}

	if(details.northRoad){
		//blank
	}
	else{
		Lot n(neighborhoodMap, map, &randomWrap, Lot::LotPosition::NORTH, Lot::LotOrientation::NS);
		n.PopulateLot();
	}

	if(!details.eastRoad && !details.northRoad){
		Lot ne(neighborhoodMap, map, &randomWrap, Lot::LotPosition::NORTHEAST, Lot::LotOrientation::EMPTY);
		ne.PopulateLot();
		Lot e(neighborhoodMap, map, &randomWrap, Lot::LotPosition::EAST, Lot::LotOrientation::EW);
		e.PopulateLot();
	}

	if(details.southRoad){
		//blank
	}
	else{
		Lot s(neighborhoodMap, map, &randomWrap, Lot::LotPosition::SOUTH, Lot::LotOrientation::NS);
		s.PopulateLot();
	}
}


