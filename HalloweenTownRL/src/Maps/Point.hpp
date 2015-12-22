/*
 * Point.hpp
 *
 *  Created on: Jul 26, 2015
 */

#ifndef POINT_HPP
#define POINT_HPP
#include <string>


class Point {
public:
	Point(uint x, uint y);
	Point(const Point& p);
	Point(const Point& p, uint x, uint y);
	virtual ~Point();

	uint getX() const;

	uint getY() const;

	int getTileIndex(int mapWidth) const;

	bool operator==(const Point &rhs);
	bool operator!=(const Point &rhs);

	std::string ToString() const;


private:
	uint x;
	uint y;
};

#endif /* POINT_HPP */


