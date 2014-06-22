#include <sys/types.h>
#include <sys/stat.h>
#include "Driver.hpp"




Game_View::Game_View(){



	for(int i =0;i<MESSAGESIZE;i++){
        messages[i]="";
    }

    heroHp = 1337;
    heroLvl = 7138;


     scrn = initscr();
    noecho();
    keypad(scrn, 1);
    mousemask(ALL_MOUSE_EVENTS, NULL);
}


/**
Refreshes the map on screen
*/
void Game_View::updateView(vector <vector<ColorChar> > newMap){
    move(0,0);
    clear();
    start_color();
    char a;
    init_pair(1, COLOR_WHITE, 0);
    init_pair(2, COLOR_RED, 0);
    init_pair(3, COLOR_GREEN, 0);
    init_pair(4, COLOR_BLACK, 0);
    init_pair(5, ColorChar::Gray, 0);
    init_pair(6, COLOR_BLUE, 0);
    int colorPairs=7;
    for(int i =0 ;i<newMap.size();i++){
        for(int j=0,t=21;j<(newMap.at(i)).size();j++, t++){
            move(i,t);
            if(newMap[i][j].getColor()==ColorChar::White){
                attron(COLOR_PAIR(1));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(1));
            }
            else if(newMap[i][j].getColor()==ColorChar::Red){
                attron(COLOR_PAIR(2));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(2));
            }
            else if(newMap[i][j].getColor()==ColorChar::Green){
                attron(COLOR_PAIR(3));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(3));
            }
            else if(newMap[i][j].getColor()==ColorChar::Black){
                attron(COLOR_PAIR(4));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(4));
            }
            else if(newMap[i][j].getColor()==ColorChar::Gray){
                attron(COLOR_PAIR(5));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(5));
            }
            else if(newMap[i][j].getColor()==ColorChar::Blue){
                attron(COLOR_PAIR(6));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(6));
            }
            else {
                init_pair(colorPairs, newMap[i][j].getColor() , 0);
                attron(COLOR_PAIR(colorPairs));
                insch(newMap[i][j].getChar());
                attroff(COLOR_PAIR(colorPairs));
                colorPairs++;
            }
        }

    }
    drawBoundries();
    displayMessages();
    displayHeroStats();
   refresh();
}
void Game_View::drawBoundries(){
    move(11,12);
    int panelWidth = 20;
    move(0,panelWidth);
    vline(ACS_VLINE,12);
    move(12,panelWidth);
    vline(ACS_VLINE,6);
    move(5,0);
    hline(ACS_HLINE,panelWidth);
    move(17,0);
    hline(ACS_HLINE,80);

}

void Game_View::updateMessages(string s){
    if(s=="")
        return;
    for(int i=MESSAGESIZE-1;i>=1;i--){
        messages[i]=messages[i-1];
    }
    messages[0]=s;
}

void Game_View::displayMessages(){
    int t =18;
    string fl="K";
    for(int i = 0;i<MESSAGESIZE;i++,t++){
        mvprintw(t ,0, messages[i].c_str());
    }
}



void Game_View::displayHeroStats(){
	Pistol p = Pistol();
    mvprintw(0,0, "Player Name");
    mvprintw(1,0, "Health: %i", heroHp);
    mvprintw(2,0, "Weapon: %s(%id%i)", p.GetDescription().c_str(), p.GetDieCount(), p.GetDieFace());
    mvprintw(3,0, "Ammo: 7/8");
    mvprintw(4,0, "Exp: %i/81%", heroLvl);
}

void Game_View::updateHeroStats(Hero h){
    heroHp=h.getHp();
    heroLvl=h.getLvl();
}//*/


void Game_View::gameoverScreen(){
    clear();
    mvprintw(10,10, "You have died...");
    getch();
    refresh();
    clear();
    echo();
    endwin();
}

