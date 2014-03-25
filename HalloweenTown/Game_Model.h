#include "TMap.h"
#include "Tile.h"
#include "Actor.h"
#include "Hero.h"
#include "Monster.h"
#include "Game_View.h"
#include<iostream>
#include<fstream>

#pragma once

#define ROWS 11
#define COLS 10

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
