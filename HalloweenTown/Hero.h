#include <time.h>
#include "Action.h"
#include "Actor.h"
#include "Items.h"
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#pragma once

class Hero : public Actor{
    private:
        string name;
        bool aimingMode;
        int aimX, aimY;
        vector<string> weapons;
        vector<string> armors;

    public:
        Hero(){}
        Hero(ColorChar c, int charId): Actor(charId) {aimingMode=false; aimX=-1; aimY=-1;}
        Hero(int charId) : Actor(charId){aimingMode=false; aimX=-1; aimY=-1;}
        Action nextAction(TMap m);
        string getName();
};
