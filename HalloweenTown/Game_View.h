#include<ncurses.h>
#include<vector>
#include "ColorChar.h"
#include "Hero.h"
#define MESSAGESIZE 6

#pragma once
using namespace std;
class Game_View{
    private:
        string messages[MESSAGESIZE];
        void displayMessages();
        void drawBoundries();
        void displayHeroStats();

        int heroHp;
        int heroLvl;
        int heroExp;
    public:
        Game_View();
        WINDOW *scrn;
        void updateView(vector < vector<ColorChar> > newMap);
	void gameoverScreen();
        void updateMessages(string s);
        void updateHeroStats(Hero h);
};
