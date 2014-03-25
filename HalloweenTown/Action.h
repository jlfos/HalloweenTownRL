#include <vector>
#include <map>
#include <string>
#pragma once

using namespace std;

class Action{
    private:
        int itemType;
        int type;
        int direction;
        int damage;
        int chance;
        int target;
        int xCord;
        int yCord;
    public:
        enum Type{
            Move,
            Attack,
            Push
        };
        Action();
        Action(int type, map<string, int> params);
        int getType();
        int getItemType();
        int getDirection();
        int getDamage();
        int getChance();
        int getTarget();
        int getXCord();
        int getYCord();

};
