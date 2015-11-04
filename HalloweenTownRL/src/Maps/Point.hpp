/*
 * Point.hpp
 *
 *  Created on: Jul 26, 2015
 */

#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
	Point(int x, int y);
	Point(const Point& p);
	virtual ~Point();

	int getX() const;

	int getY() const;

	int getTileIndex(int mapWidth) const;

	bool operator==(const Point &rhs);
	bool operator!=(const Point &rhs);


private:
	int x;
	int y;
};

#endif /* POINT_HPP */
