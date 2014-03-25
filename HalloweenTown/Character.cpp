//#include <Character.h>
//#include <math.h>
//#include <stdlib.h>
//#include <cstddef>
//#include <Actor.h>
// FIX!!!!
//Character::Character(int reference){
//
//    if (reference == 0){
//
//        lvl = 1; //Default: 1
//        exp = 0;
//        range = 1; //determined by weapon; Default: 1
//
//        str = diceRoll(6, 3); //18 max attribute
//        dex = diceRoll(6, 3);
//        con = diceRoll(6, 3);
//        wil = diceRoll(6, 3);
//        chr = diceRoll(6, 3);
//        intelligence = diceRoll(6, 3);
//
//        int spd = (dex + con)/2; //average of dex and con
//        int hp = (str + con)/2; //average of str and con
//        int attack = (str + dex)/2; //average of str and dex
//    }
//
//    else {
//
//        lvl = 1; //Default: 1
//        exp = 0;
//        range = 1; //determined by weapon; Default: 1
//
//        str = diceRoll(6, 3); //18 max attribute
//        dex = diceRoll(6, 3);
//        con = diceRoll(6, 3);
//        wil = diceRoll(6, 3);
//        chr = diceRoll(6, 3);
//        intelligence = diceRoll(6, 3);
//
//        int spd = (dex + con)/2; //average of dex and con
//        int hp = (str + con)/2; //average of str and con
//        int attack = (str + dex)/2; //average of str and dex
//
//        inventory[20];
//    }
//}
//
//
//int Character::diceRoll(int face, int diceCount){
//        srand(time(NULL));
//        int r = 0;
//
//        for(int i = 0; i<diceCount; i ++){
//            r += (rand() % face);
//        }
//        return r;
//}
//
//void itemAdd(){
//}
