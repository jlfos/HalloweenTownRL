#include<ncurses.h>
#include<string>
#include "Game_View.h"
#include "Tile.h"
#include "Game_Model.h"
#define ROWS 11
#define COLS 10

//Test commit
int main(){
    int d;
    Game_View v= Game_View();
    init_color(COLOR_RED, 700, 0, 0);

    Game_Model mod("./example.txt");
    mod.setView(&v);
    mod.updateMap();

    while(mod.play()){

    }
    return 0;
}
