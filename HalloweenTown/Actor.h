#include <time.h>
#include "Action.h"
#include "TMap.h"
#include <stdio.h>
#include <curses.h>



#pragma once
using namespace std;

class Actor{
    protected:
        char c;
        int lvl; //Default: 1
        int exp; // determines level
        int range; //determined by weapon; Default: 1

        int str; //18 max attribute
        int dex;
        int con;
        int wil;
        int chr;
        int intelligence;
        int reference;
        int spd; //average of dex and con
        int attack; //average of str and dex

        /** Inventory*/
        int armour;
        int weapon;
        int inventory[]; //numbers associated with item lookup table

    public:
        int hp; //average of str and con
        int diceRoll(int face, int diceCount);
        int itemAdd();
        void itemMod();
        Actor();
        int incomingAttack(int chance, int damage);
        Actor(int charId);
        Actor(ColorChar c, int i);
        void setLvl(int i);
        void setHp(int i);
        void setStr(int i);
        void setDex(int i);
        void setCon(int i);
        void setWil(int i);
        void setChr(int i);
        void setIntelligence(int i);
        int getHp();
        int getLvl();

        virtual Action nextAction(TMap m);
};



