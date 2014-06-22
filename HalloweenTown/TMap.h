//#include "Driver.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
#pragma once
class TMap{
    private:
        bool illegalBulletPath(int x, int y);
        vector <vector<Tile> > tileMap;
        bool loadMap(char *filname);
        bool obstacle;
        void addBulletPath(ColorChar c, int x, int y);
        int rows;
        int cols;
        typedef struct Point
        {   ColorChar c;
            int x;
            int y;};
        vector<Point> charList;
        vector<Point> trajectory;

    public:
        enum Direction{
            Up=1,
            Right,
            Down,
            Left
        };

        bool legalMove(int x1, int x2, int direction);
        bool isSpaceOccupied(int x, int y);
        int heroX;
        int heroY;
        TMap(char *filename);
        TMap();
        int getId(int x, int y);
        Tile getTile(int x, int y);
        void moveChar(int x, int y, int dir);
        void moveChar(int characterID, int dir);
        vector<vector<ColorChar> > TMapConvert();
        vector<ColorChar> getChars();
        int getCharXCord(int charId);
        int getCharYCord(int charId);
        void killCharacter(int charId);
        void drawTrajectory(int targetX, int targetY);
        int showBulletPath();
        char getChar(int x, int y);
        void setChar(char c, int x, int y, int lvl);
        int distance(int x1, int y1, int x2, int y2);
        bool legalTile(int x, int y);
        void clearOverlay();
        void look();
        void fogOfWar();
        ColorChar getDropLvl(int x, int y);
};
