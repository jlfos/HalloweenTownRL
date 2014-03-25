#include <Action.h>

Action::Action(){
}

/**Constructor for action.
t- type of action, current range is 0-4:
0 movement; params needed: dir, ref
1 melee attack; params needed dam, cha, ,ref and tar
2 aiming ranged attack; params needed xco, yco
3 ranged attack; params needs xco, yco, dam, cha
4 open door; params needed xco, yco
5 nothing; no params
6 pick up item - params, xco, yco, item type
params - map object used to store parameters for the specialized actions.*/
Action::Action(int t, map<string, int> params){
    switch(t){
        case 0: //move
            type = t;
            direction = params["dir"];
            damage = 0;
            chance = 0;
            break;
        case 1: //melee attack
            type = t;
            damage = params["dam"];
            chance = params["cha"];
            target = params["tar"];
            break;
        case 2: //aiming a ranged attack
            type = t;
            xCord = params["xco"];
            yCord = params["yco"];
            break;
        case 3:
            type = t;
            damage = params["dam"];
            chance = params["cha"];
            xCord = params["xco"];
            yCord = params["yco"];
            break;
        case 4:
            type = t;
            xCord = params["xco"];
            yCord = params["yco"];
            break;
        case 5:
            type =t;
            break;
        case 6:
            type =t;
            itemType = params["ite"];
            xCord = params["xco"];
            yCord = params["yco"];
            break;
        default: break;
    }
}

int Action::getType(){
    return type;
}




int Action::getDirection(){
    return direction;
}


int Action::getDamage(){
    return damage;
}

int Action::getTarget(){
    return target;
}

int Action::getChance(){
    return chance;
}

int Action::getXCord(){
    return xCord;
}

int Action::getYCord(){
    return yCord;
}

int Action::getItemType(){
    return itemType;
}
