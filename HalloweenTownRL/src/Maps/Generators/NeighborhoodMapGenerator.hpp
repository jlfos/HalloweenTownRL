/*
 * NeighborhoodMapGenerator.hpp
 *
 *  Created on: Nov 9, 2015
 */

#ifndef NEIGHBORHOODMAPGENERATOR_HPP
#define NEIGHBORHOODMAPGENERATOR_HPP

#include "libtcod.hpp"
#include "MapGenerator.hpp"
#include "../Point.hpp"
#include "../Room.hpp"
#include "../../RandomWrapper.hpp"
struct NeighborhoodDetails{
	bool westRoad = false;
	bool eastRoad = false;
	bool northRoad = false;
	bool southRoad = false;
};



class NeighborhoodMapGenerator : public MapGenerator {

private:
	u_int mapWidth;
	u_int mapHeight;
	int minRoomSizeX;
	int minRoomSizeY;
	int maxRoomSizeX;
	int maxRoomSizeY;
	int lotSizeX;
	int lotSizeY;
	int lotDesignator;
	std::vector<int> furniture;
	int lampPostRate;
	bool backDoor;
	MapGenerator::Orientation mapOri;
	Map* map;
	TCODMap* neighborhoodMap;
	int treeChance;
	NeighborhoodDetails details;


public:
	NeighborhoodMapGenerator(int width, int height, MapGenerator::Orientation orientation, NeighborhoodDetails details);
	~NeighborhoodMapGenerator();
	void PopulateActors(Map* map);
	TCODMap* Generate(Map* map, bool generateActors);

private:

	RandomWrapper randomWrap;
	void DrawDoor(const Point& door);
	void DrawVerticalSidewalk(Point start, Point end, bool evenLampposts);
	void DrawHorizontalSidewalk(Point start, Point end, bool evenLampposts);
	void DrawRoads();
	void DrawSidewalks();
	void DrawHouses();
	void DrawNextDoor(Room* ra);
	void CreateHouse(int lotX, int lotY, MapGenerator::Orientation side,TCODColor visible);

};



#endif /* NEIGHBORHOODMAPGENERATOR_HPP */
