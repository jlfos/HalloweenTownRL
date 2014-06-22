#include<ncurses.h>
#include<string>
#include "Driver.hpp"
#define ROWS 11
#define COLS 10


int main(){



	Game_View v= Game_View();
    init_color(COLOR_RED,700,0,0);

    Game_Model mod("./example.txt");
    mod.setView(&v);
    mod.updateMap();

    while(mod.play()){

    }
    return 0;
}
