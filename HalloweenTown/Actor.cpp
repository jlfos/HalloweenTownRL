#include "Driver.hpp"
Actor::Actor(){
}

Actor::Actor(int charId){
    reference = charId;
    if( reference == 0){

        lvl = 1; //Default- 1
        exp = 0;
        range = 1; //determined by weapon; Default: 1

        str = diceRoll(6, 3); //18 max attribute
        dex = diceRoll(6, 3);
        con = diceRoll(6, 3);
        wil = diceRoll(6, 3);
        chr = diceRoll(6, 3);
        intelligence = diceRoll(6, 3);

        int spd = (dex + con)/2; //average of dex and con
        hp = 100; //average of str and con
        int attack = (str + dex)/2; //average of str and dex
    }

    else {

        lvl = 1; //Default: 1
        exp = 0;
        range = 1; //determined by weapon; Default: 1

        str = diceRoll(6, 3); //18 max attribute
        dex = diceRoll(6, 3);
        con = diceRoll(6, 3);
        wil = diceRoll(6, 3);
        chr = diceRoll(6, 3);
        intelligence = diceRoll(6, 3);

        int spd = (dex + con)/2; //average of dex and con
        hp = 13; //average of str and con
        int attack = (str + dex)/2; //average of str and dex

        inventory[20];
    }
}

Actor::Actor(ColorChar c, int i){
    hp = 10;
    reference = i;
}

/**
simulates dice roll.
face - number of faces on simulated die
diceCount - number of dice being rolled
returns summation of dice rolls*/
int Actor::diceRoll(int face, int diceCount){
        srand(time(NULL));
        int r = 0;
        for(int i = 0; i<diceCount; i ++){
            r += (rand() % face);
        }
        return r;
}
/**
asks for the next action from the user.
m - state of the current map
recieves input from the user and creates an action object based on the input.
*/
Action Actor::nextAction(TMap m){
return Action();
}


int Actor::incomingAttack(int chance, int damage){
    hp-=damage;
    if (hp<=0)
        return -1;
    else
        return 0;
}


int Actor::getHp(){
    return hp;
}

int Actor::getLvl(){
    return lvl;
}
