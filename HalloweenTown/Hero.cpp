#include "Driver.hpp"

Action Hero::nextAction(TMap m){
    int move;
    int heroX = m.getCharXCord(0);
    int heroY = m.getCharYCord(0);
    move = getch();
    map<string, int> params;
    if(move=='F'&&aimingMode){
        aimingMode=false;
        return Action(5, params);
    }
    else if(move=='f'){
        if(aimingMode){
            aimingMode=false;
            params.insert(pair<string,int>("xco", aimX ));
            params.insert(pair<string,int>("yco", aimY ));
            params.insert(pair<string,int>("dam", 7 ));
            params.insert(pair<string,int>("cha", 2 ));
            return Action(3,params);
        }

        else{
            aimingMode=true;
            if(aimX==-1&&aimY==-1){
                aimX=heroX+1;
                aimY=heroY+1;
            }
            params.insert(pair<string,int>("xco", aimX ));
            params.insert(pair<string,int>("yco", aimY ));
            return Action(2,params);
        }
    }
    else if(move=='g'){
        Items list= Items();
        ColorChar temp = m.getDropLvl( heroX, heroY);
        if(temp.getChar()=='\0'){
            params["ite"]=0;
            return Action(6, params);
        }
        else if(temp.getChar()==')' ){
            if(temp.getColor()==ColorChar::Gold){
                params["ite"]=1;
                params["xco"]=heroX;
                params["yco"]=heroY;
                weapons.push_back("shotgun");
                return Action(6, params);
            }
            if(temp.getColor()==ColorChar::DkGreen){
                params["ite"]=2;
                params["xco"]=heroX;
                params["yco"]=heroY;
                weapons.push_back("pistol");
                return Action(6, params);
            }

        }
    }//*/
    else if(move==KEY_UP){
        if(aimingMode ){                                                //aim up
            if( m.legalTile(aimX, aimY-1) )
                aimY-=1;
            params.insert(pair<string,int>("xco", aimX ));
            params.insert(pair<string,int>("yco", aimY ));
            return Action(2,params);
        }
        else if(m.getChar(heroX, heroY-1) =='+'){  //Open door up
                params.insert(pair<string, int>("xco", heroX));
                params.insert(pair<string, int>("yco", heroY-1 ));
                return Action(4, params);
        }

        else if(m.isSpaceOccupied( m.getCharXCord(reference), m.getCharYCord(reference)-1 )){               //punch up
            diceRoll(3, 4);
            params.insert(pair<string,int>("dam", 5) );
            params.insert(pair<string,int>("cha", 1) );
            params.insert(pair<string,int>("tar", m.getId(m.getCharXCord(reference), m.getCharYCord(reference)-1 ) ) );
            return Action(1, params);
        }
        else{                                                           //move up
            params.insert(pair<string,int>("dir", TMap::Up) );
            return Action(0, params);
        }
    }
    else if(move==KEY_DOWN){
        if(aimingMode){                 //aim down
            if( m.legalTile(aimX, 1+aimY) )
                aimY+=1;

            params.insert(pair<string,int>("xco", aimX ));
            params.insert(pair<string,int>("yco", aimY ));
            return Action(2,params);
        }

        else if(m.getChar(heroX, heroY+1) =='+'){  //Open door down
                params.insert(pair<string, int>("xco", heroX));
                params.insert(pair<string, int>("yco", heroY+1 ));
                return Action(4, params);
        }
        else if(m.isSpaceOccupied( m.getCharXCord(reference), m.getCharYCord(reference)+1) ){  //Punch down

            diceRoll(3, 4);
            params.insert(pair<string,int>("dam", 5) );
            params.insert(pair<string,int>("cha", 1) );
            params.insert(pair<string,int>("tar", m.getId(m.getCharXCord(reference), m.getCharYCord(reference)+1 ) ) );
            return Action(1, params);
        }

        else{                                                                   //move down
            params.insert(pair<string,int>("dir", TMap::Down) );
            return Action(0, params);
        }
    }
    else if(move==KEY_LEFT){
        if(aimingMode){                                                 //aim left
            if( m.legalTile(aimX-1, aimY) )
                aimX-=1;
            params.insert(pair<string,int>("xco", aimX ));
            params.insert(pair<string,int>("yco", aimY ));
            return Action(2,params);
        }
        else if(m.getChar(heroX-1, heroY) =='+'){  //Open door left
                params.insert(pair<string, int>("xco", heroX-1));
                params.insert(pair<string, int>("yco", heroY ));
                return Action(4, params);
        }

        else if(m.isSpaceOccupied( m.getCharXCord(reference)-1, m.getCharYCord(reference) )){               //punch left
            diceRoll(3, 4);
            params.insert(pair<string,int>("dam", 5) );
            params.insert(pair<string,int>("cha", 1) );
            params.insert(pair<string,int>("tar", m.getId(m.getCharXCord(reference)-1, m.getCharYCord(reference) ) ) );
            return Action(1, params);
        }

        else{
            params.insert(pair<string,int>("dir", TMap::Left) );                                    //move left
            return Action(0, params);
        }
    }
    else if(move==KEY_RIGHT){
        if(aimingMode){                                                             //aim right
            if( m.legalTile(aimX+1, aimY) )
                aimX+=1;
            params.insert(pair<string,int>("xco", aimX ));
            params.insert(pair<string,int>("yco", aimY ));
            return Action(2,params);
        }

        else if(m.getChar(heroX+1, heroY) =='+'){  //Open door left
                params.insert(pair<string, int>("xco", heroX+1));
                params.insert(pair<string, int>("yco", heroY ));
                return Action(4, params);
        }


        else if(m.isSpaceOccupied( m.getCharXCord(reference)+1, m.getCharYCord(reference) )){           //punch right
            diceRoll(3, 4);
            params.insert(pair<string,int>("dam", 5) );
            params.insert(pair<string,int>("cha", 1) );
            params.insert(pair<string,int>("tar", m.getId(m.getCharXCord(reference)+1, m.getCharYCord(reference) ) ) );
            return Action(1, params);
        }


        else {
            params.insert(pair<string,int>("dir", TMap::Right) );                       //move right
            return Action(0, params);
        }
    }
}

string Hero::getName(){
    return "Player";
}
