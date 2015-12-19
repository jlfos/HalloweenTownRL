/*
 * RoadMapGenerator.hpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */

#ifndef HALLOWEENTOWN_ROADMAPGENERATOR_HPP
#define HALLOWEENTOWN_ROADMAPGENERATOR_HPP

#include <memory>
#include "libtcod.hpp"
#include "Generators/MapGenerator.hpp"
#include "Point.hpp"
#include "../RandomWrapper.hpp"
#include "Room.hpp"
class Map;
class RandomWrapper;

class RoadMapGenerator : public MapGenerator{
	public:
		RoadMapGenerator();
		RoadMapGenerator(MapGenerator::Orientation orientation);
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);


	private:
		void GenerateTile(int x, int y, int width, int height, TCODMap* roadMap);
		void DrawRoad(int x, int y, TCODMap* roadMap);
		void DrawGrass(int x, int y, TCODMap* roadMap);
		void AddItem(Map* map, int x, int y);
		MapGenerator::Orientation flagOri;
		Map* map;
		TCODMap* roadMap;
		int minSizeX;
		int minSizeY;
		int maxSizeX;
		int maxSizeY;
};


#endif /* HALLOWEENTOWN_ROADMAPGENERATOR_HPP */
