//#include "Driver.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#pragma once

#define ROWS 11
#define COLS 10
using namespace std;
class Game_Model {
    private:
        int aimX,aimY;
        Game_View *view;
        Hero theHero;
        bool monstersMove;
        vector<Monster> monsterList; //always ignore first entry for monsterList. it is a place holder.
        void characterActorSync();
    public:
        int play();
        TMap gameMap;
        void input(int move);
        Game_Model(string map[]);
        void updateMap();
        void setView(Game_View *view);
        Game_Model(char *filePath);
        string interpretAction(int charId, Action a);
        void removeMonster(int reference);
};
