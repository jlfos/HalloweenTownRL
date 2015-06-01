/*
 * RoadMapGenerator.hpp
 *
 *  Created on: Feb 28, 2015
 *      Author: josh
 */

#ifndef ROADMAPGENERATOR_HPP_
#define ROADMAPGENERATOR_HPP_


class Map;

class RoadMapGenerator : public MapGenerator{
	public:
		RoadMapGenerator();
		RoadMapGenerator(MapGenerator::Orientation orientation);
		TCODMap* Generate(Map* map, bool generateActors);
		void PopulateActors(Map* map);
	private:
		void GenerateTile(int x, int y, int width, int height, TCODMap* roadMap, Map* map);
		void GenerateRoad(int x, int y, int width, TCODMap* roadMap, Map* map);
		void GenerateGrass(int x, int y, int width, TCODMap* roadMap, Map* map);
		void AddItem(Map* map, int x, int y);
		MapGenerator::Orientation orientation;
};


#endif /* ROADMAPGENERATOR_HPP_ */
