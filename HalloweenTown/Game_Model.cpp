#include "Game_Model.h"

/**

Constructor for Game Model. Takes the filepath to map file.
( '.'=ground, '#'=wall, '@'=hero)
Starts the hero off the map at -1,-1 and scans the map
until it finds the hero character. Once it finds the hero
it updates the hero coordinates accordingly
*/
Game_Model::Game_Model(char *filePath)
{
	Pistol p = Pistol();
    gameMap = TMap(filePath);
    characterActorSync();
    monstersMove = true;
}



/**
takes the current instance of the map and gives it the view to
draw it.
*/
void Game_Model::updateMap()
{
	gameMap.fogOfWar();
    gameMap.look();
    vector< vector <ColorChar> > printTheMap = gameMap.TMapConvert();
    (*view).updateHeroStats(theHero);
    (*view).updateView(printTheMap);
}



void Game_Model::setView(Game_View *viewN)
{
    view=viewN;
}

/**
Cycles through all of the characters on screen asking for an action from each of them.
Takes the action, interpretes it and makes the appropriate changes to the map based on
the action.
*/
int Game_Model::play()
{
    Action tempAct = theHero.nextAction(gameMap);
    string actionResult =interpretAction(0, tempAct);
    (*view).updateMessages(actionResult);
    if (monstersMove)
    {
        for (int i =1; i<monsterList.size();i++)
        {
            Action tempAct = (monsterList.at(i)).nextAction(gameMap);
            string tempStr = interpretAction(i, tempAct);

            if(tempStr=="G")
                return 0;
            (*view).updateMessages(tempStr);
        }

    }

    updateMap();
    return 1;
}

/**
Takes an action object(and the id of the actor who generated the object) and
interprets what changes should be made to the map based on the action.
charId - number for the character that generated the action object
a - the action object
return - a string indicating the result of the attempted action
*/
string Game_Model::interpretAction(int charId, Action a)
{

    switch (a.getType())
    {
    case 0:
    {
        gameMap.moveChar(charId, a.getDirection());
        monstersMove=true;
        return "";
    }
    break;
    case 1:

    {
    	Pistol p = Pistol();
    	int damage = p.GetDamage();
    	int attack;
        monstersMove=true;
        if(a.getTarget()==0){
            attack = theHero.incomingAttack(5,5);
        }
        else
            attack =(monsterList.at(a.getTarget())).incomingAttack(5, damage);

        if (attack==1)
        {
            string ret = "The ? misses the ?";
            ret.insert(4, theHero.getName());
            return "Attack missed";
        }
        else if (attack==0)
        {
            string ret = "The  hits the .";
            if(a.getTarget()==0){
                string tempStr = (monsterList.at(charId)).getName();
                ret.insert(4, tempStr );
                ret.insert(14+tempStr.length(), theHero.getName());
                return ret;
            }
            else{
                ret.insert(4, theHero.getName());
                ret.insert(20, (monsterList.at(a.getTarget() )).getName());
                return ret;
            }
        }
        else if (attack==-1)
        {
            string ret = "The  dies.";
            if (a.getTarget()!=0){
	            ret.insert(4,(monsterList.at(a.getTarget())).getName());
	            gameMap.killCharacter(a.getTarget());
	            removeMonster(a.getTarget());
	            //monsterList.erase(monsterList.begin()+a.getTarget());
	            return ret;
            }
            else{
                (*view).gameoverScreen();
                return "G";
            }
        }
    }
    break;
    case 2:
    {
        monstersMove = false;
        gameMap.drawTrajectory((int)a.getXCord(),(int)a.getYCord());
        return "";
        break;
    }
    case 3:
    {
        int feedback = gameMap.showBulletPath();
        while (feedback==-1)
        {
            updateMap();
            usleep(15000);
            feedback = gameMap.showBulletPath();
        }
        monstersMove = true;

        if (feedback>0)
        {

            int attack = (monsterList.at(feedback)).incomingAttack(5, 5);
            if (attack==1)
            {
                string ret = "The ? misses the ?";
                ret.insert(4, theHero.getName());
                return "Attack missed";
            }
            else if (attack==0)
            {
                string ret = "The  shoots the .";
                ret.insert(4, theHero.getName());
                ret.insert(22, (monsterList.at(feedback)).getName());
                return ret;
            }
            else if (attack==-1)
            {
                string ret = "The  dies.";
                ret.insert(4,(monsterList.at(feedback).getName()));
                gameMap.killCharacter(feedback);
                removeMonster(feedback );
                //monsterList.erase(monsterList.begin()+feedback);
                return ret;
            }
        }
    }
        break;
        case 4:{
            monstersMove=true;
            gameMap.setChar('\0', a.getXCord(), a.getYCord(), Tile::charlvl  );
            return "";
        }
        break;

        case 5:{
            monstersMove = false;
            gameMap.clearOverlay();
            updateMap();
            return "";
        }
        break;
        case 6:{
            if(a.getItemType()==0)
                return "There is no item to pick up";
            else if(a.getItemType()==1){
                gameMap.setChar('\0', a.getXCord(), a.getYCord(), Tile::droplvl  );
                return "You picked up a shotgun";
            }
            else if(a.getItemType()==2){
                gameMap.setChar('\0', a.getXCord(), a.getYCord(), Tile::droplvl  );
                return "You picked up a pistol";
            }
        }
        break;
    }
}

/**
Takes the characters in 'TMap gameMap' and converts them into a vector of ColorChars.
*/
void Game_Model::characterActorSync()
{
	vector<ColorChar> tempVec = gameMap.getChars();
    monsterList.clear();
    theHero = Hero(0);
    for (int i =0; i<tempVec.size(); i++)
    {
        monsterList.push_back(Monster(tempVec.at(i), i )); //always ignore first entry for monsterList. it is a place holder.
    }
}

void Game_Model::removeMonster(int reference){
    monsterList.erase(monsterList.begin()+reference);
    for(int i = reference; i<monsterList.size();i++){
        monsterList.at(i).lowerReference();
    }
    return;
}
