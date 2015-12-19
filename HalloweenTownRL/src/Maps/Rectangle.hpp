/*
 *  Rectangle.hpp
 *
 *  Created on: Dec 19, 2015
 */

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "../Tile/TileCharacters.hpp"
#include "Map.hpp"
struct RectangleDetails{
	int neCorner = TileCharacters::Default::RAINBOW;
	int northSide = TileCharacters::Default::RAINBOW;
	int nwCorner = TileCharacters::Default::RAINBOW;
	int westSide = TileCharacters::Default::RAINBOW;
	int swCorner = TileCharacters::Default::RAINBOW;
	int southSide = TileCharacters::Default::RAINBOW;
	int seCorner = TileCharacters::Default::RAINBOW;
	int eastSide = TileCharacters::Default::RAINBOW;
};


class Rectangle {
public:
	Rectangle(Point nwCorner, Point seCorner, TCODColor color, int character);
	Rectangle(Point nwCorner, Point seCorner, TCODColor color, RectangleDetails details);
	void Draw(Map* map);

private:
	Point nwCorner;
	Point seCorner;
	TCODColor color;
	RectangleDetails details;
	void DrawNorthSide(Map* map);
	void DrawEastSide(Map* map);
	void DrawSouthSide(Map* map);
	void DrawWestSide(Map* map);





};


#endif /* RECTANGLE_HPP */
