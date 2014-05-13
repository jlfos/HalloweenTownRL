#include "ColorChar.h"
#include <ncurses.h>
ColorChar::ColorChar(char c, int i){
    character=c;
    color = i;
    init_color(8, 170,170,170);
    init_color(9, 255*3,185*3,15*3);
    init_color(10, 66*4 ,111*4,66*4);
}

ColorChar::ColorChar(char c){
    character=c;
    color = White;
}

ColorChar::ColorChar(){}


int ColorChar::getColor(){
    return color;
}

void ColorChar::setColor(int i){
    color = i;
}

char ColorChar::getChar(){
    return character;
}

void ColorChar::setChar(char c){
    character = c;
    return;
}


