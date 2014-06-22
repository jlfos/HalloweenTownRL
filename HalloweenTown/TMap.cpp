#include "Driver.hpp"
#include <string>

#include <iostream>
#include <fstream>
#include <vector>
TMap::TMap(){}

TMap::TMap(char *filename){

    heroX=-1;
    heroY=-1;
    loadMap(filename);
}

/**
filename - filepath/filename of the map to load
returns - true or false based on if it was able to load the file or not.
Loads a map from the file contained a 'filename'.
Reads in the the file found at 'filename' line by line and converts it
to a 2-dimensional vector of chars which will become the new theMap field vairable.
NOTE: only include the .txt extension for the first map you load in a run instance. after
that do NOT include the .txt extension.
*/
bool TMap::loadMap(char *filename){
    ifstream myfile;
    myfile.open (filename, ifstream::in);
    if (myfile.is_open())
    {
        tileMap.clear();
        vector<ColorChar> textFile;
        string tempStr;
        Tile tempTile;
        rows=0;
        cols=0;
        int temp=0;
        while (!myfile.eof())
        {
            rows++;
            vector<Tile> tempVec;
            getline(myfile, tempStr);
            temp=0;
            for (int i = 0; i<tempStr.length();i++, temp++)
            {   Tile tempTile;
                tempVec.push_back(tempTile);
                if(tempStr.at(i)=='@'){
                    tempTile.setBackLvl('.');
                    tempTile.setCharLvl(ColorChar(tempStr.at(i), ColorChar::Green));
                    tempTile.setFog('.');
                    heroX=i-1;
                    heroY=rows-1;
                    Point p = {ColorChar('@') ,i, heroY};
                    charList.insert(charList.begin(), p );  //Ensures hero is at 0 position on charList
                }
                else if(tempStr.at(i)=='g'){
                    tempTile.setBackLvl('.');
                    tempTile.setCharLvl(ColorChar('g', ColorChar::Blue));
                    tempTile.setFog('.');

                    tempTile.setFogFlag(true);
                    charList.push_back({ColorChar('g') ,i, rows-1});

                }
                else if(tempStr.at(i)=='+'){
                    tempTile.setBackLvl('/');
                    tempTile.setCharLvl('+');
                    tempTile.setFog('+');
                }
                else if(tempStr.at(i)==')'){
                    tempTile.setFog('.');
                    tempTile.setBackLvl('.');
                    tempTile.setDropLvl(ColorChar(')', ColorChar::Gold) );
                    tempTile.setFogFlag(true);
                }
                else if(tempStr.at(i)=='.'||tempStr.at(i)=='#'){
                    tempTile.setBackLvl(tempStr.at(i));
                    tempTile.setFogFlag(true);
                    tempTile.setFog(tempStr.at(i));
                }
                tempVec.back()=tempTile;
            }
            if (temp>cols)
                cols=temp;
            if (tempVec.size()>0)
                tileMap.push_back(tempVec);
        }
        rows--;
        myfile.close();
        return true;
    }
    else
        return false;
}

vector<vector<ColorChar> > TMap::TMapConvert(){
    vector<vector<ColorChar> > ret;
    for(int i = 0; i<tileMap.size();i++){
        vector<ColorChar> tempVec;
        ret.push_back(tempVec);
        for(int j = 0; j< tileMap.at(i).size(); j++){
            ret.at(i).push_back(( (tileMap.at(i)).at(j)).getHighest() );


        }
    }
    return ret;
}

void TMap::moveChar(int x, int y, int dir){


    if((((tileMap.at(y)).at(x)).getHighest()).getChar()=='\0' || !legalMove(x,y,dir))
        return;
    ColorChar temp;
    temp = ((tileMap.at(y)).at(x)).getHighest();
    ((tileMap.at(y)).at(x)).setCharLvl('\0');
    ((tileMap.at(y)).at(x)).revert();
    switch(dir){
        case 1:
            y--;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            break;
        case 2:
            x++;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            break;
        case 3:
            y++;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            break;
        case 4:
            x--;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            break;
        default:
            break;
    }
}

void TMap::moveChar(int characterID,  int dir){
    int x = charList.at(characterID).x;
    int y = charList.at(characterID).y;
    if((((tileMap.at(y)).at(x)).getHighest()).getChar()=='\0' || !legalMove(x,y,dir))
        return;
    ColorChar temp;
    temp = ((tileMap.at(y)).at(x)).getCharLvl();
    ((tileMap.at(y)).at(x)).setCharLvl('\0');
    ((tileMap.at(y)).at(x)).revert();
    switch(dir){
        case 1:
            y--;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            charList.at(characterID).y--;
            break;
        case 2:
            x++;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            charList.at(characterID).x++;
            break;
        case 3:
            y++;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            charList.at(characterID).y++;
            break;
        case 4:
            x--;
            ((tileMap.at(y)).at(x)).setCharLvl(temp);
            charList.at(characterID).x--;
            break;
        default:
            break;
    }
}




bool TMap::legalMove(int x, int y, int dir){



    switch(dir){

        case 1:
            y--;
            return (((tileMap.at(y)).at(x)).getHighest()).getChar()!='#' &&
                    (((tileMap.at(y)).at(x)).getCharLvl()).getChar()=='\0';
            break;
        case 2:
            x++;
            return (((tileMap.at(y)).at(x)).getHighest()).getChar()!='#' &&
                    (((tileMap.at(y)).at(x)).getCharLvl()).getChar()=='\0';
            break;
        case 3:
            y++;
            return (((tileMap.at(y)).at(x)).getHighest()).getChar()!='#' &&
                    (((tileMap.at(y)).at(x)).getCharLvl()).getChar()=='\0';
            break;
        case 4:
            x--;
            return (((tileMap.at(y)).at(x)).getHighest()).getChar()!='#' &&
                    (((tileMap.at(y)).at(x)).getCharLvl()).getChar()=='\0';
            break;
        default:
            return true;
    }
}

bool TMap::isSpaceOccupied(int x, int y){
    return (((tileMap.at(y)).at(x)).getCharLvl()).getChar()!='\0';
}

int TMap::getId(int x, int y){
    for(int i =0; i<charList.size(); i++){
        if(charList.at(i).x==x && charList.at(i).y==y )
            return i;
    }
}


int TMap::getCharXCord(int charId){
    return charList.at(charId).x;
}

int TMap::getCharYCord(int charId){
    return charList.at(charId).y;
}

vector<ColorChar> TMap::getChars(){
    vector<ColorChar> ret;
    for(int i = 0; i<charList.size(); i++){
        ret.push_back((charList.at(i)).c);
    }
    return ret;
}


/**
removes the character by erasing it from the charList and replacing its ColorChar on the map with a corpse.
charId - the id of the character to kill
*/
void TMap::killCharacter(int charId){
    tileMap[getCharYCord(charId)][getCharXCord(charId)].setCharLvl('\0');
    tileMap[getCharYCord(charId)][getCharXCord(charId)].setBackLvl(ColorChar('%', ColorChar::Red) );
    charList.erase(charList.begin()+charId);
}


/**
Determines the bullet path from the hero to the target. There are 12 different areas the
hero could be aiming:
             N
      \   8  |  1   /
        \    |    /
       7  \  |  /   2
           _\|/
     W-------+--------E
       6    /|\
          /  |  \   3
        / 5  | 4  \
      /      |      \
             S

targetX - the x coordinate of the target
targetY - the y coordinate of the target
*/
void TMap::drawTrajectory(int targetX, int targetY){
    clearOverlay();
    trajectory.clear();
    int heroX = getCharXCord(0);
    int heroY = getCharYCord(0);
    int tempX =heroX;
    int tempY =heroY;
    int color = ColorChar::Green;
                                                /** Start of cardinal directions*/
    if(targetX==heroX&&targetY>heroY){ //S
        tempY++;

        for(int j=0;j<targetY-heroY;j++,tempY++){
            if(illegalBulletPath(tempX, tempY)){
                color = ColorChar::Red;
            }
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
            tileMap[tempY][tempX].setFogFlag(false);
            if(color==ColorChar::Green)
                addBulletPath(ColorChar('|'), tempX,tempY);
        }
        tempY--;
        tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
        return;
    }
    else if(targetX==heroX&&targetY<heroY){  //N
        tempY--;
        for(int j=0;j<heroY-targetY;j++,tempY--){
            if(illegalBulletPath(tempX, tempY)){
                color = ColorChar::Red;
            }
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
            tileMap[tempY][tempX].setFogFlag(false);
            if(color==ColorChar::Green)
                addBulletPath(ColorChar('|'), tempX,tempY);
        }
        tempY++;
        tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
        return;
    }
    else if(targetX>heroX&&targetY==heroY){ //E

        tempX++;
        for(int j=0;j<targetX-heroX;j++,tempX++){
            if(illegalBulletPath(tempX, tempY)){
                color = ColorChar::Red;
            }
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
            tileMap[tempY][tempX].setFogFlag(false);
            if(color==ColorChar::Green)
                addBulletPath(ColorChar('-'), tempX,tempY);
        }
        tempX--;
        tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
        return;
    }
    else if(targetX<heroX&&targetY==heroY){ //W
        tempX--;
        for(int j=0;j<heroX-targetX;j++,tempX--){
            if(illegalBulletPath(tempX, tempY)){
                color = ColorChar::Red;
            }
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
            tileMap[tempY][tempX].setFogFlag(false);
            if(color==ColorChar::Green)
                addBulletPath(ColorChar('-'), tempX,tempY);
        }
        tempX++;
        tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
        return;
    }

    else if(targetX>=heroX&&targetY>=heroY){ /**start of 3 & 4*/
        if(targetX-heroX>=targetY-heroY){ //3
            int delim =targetY-heroY;
            int allocation[delim];
            for(int z = 0; z<delim;z++)
                allocation[z]=0;

            for(int j = 0, i =0;i<targetX-heroX;j++){
                    allocation[j%(delim)]+=1;
                    i++;
            }
            tempX++;
            tempY++;
            for(int i=0;i<delim;i++,tempY++){
                if(color==ColorChar::Green)
                    addBulletPath(ColorChar('|'), tempX,tempY);
                for(int j=0;j<allocation[i];j++,tempX++){
                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }
                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('-'), tempX,tempY);

                }
            }
            tempX--;
            tempY--;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;
        }

        else if(targetY-heroY>=targetX-heroX){  //4

            int delim =targetX-heroX;
            int allocation[delim];
            for(int z = 0; z<delim;z++)
                allocation[z]=0;
            for(int j =0,i=0;j<targetY-heroY;j++){
                allocation[j%(delim)]+=1;
                i++;
            }

            tempX++;
            tempY++;
            for(int i=0;i<delim;i++,tempX++){
                if(color==ColorChar::Green)
                    addBulletPath(ColorChar('-'), tempX,tempY);
                for(int j=0;j<allocation[i];j++,tempY++){
                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }

                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('|'), tempX,tempY);


                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                }
            }
            tempX--;
            tempY--;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;
        }
    }
    else if(targetX<=heroX&&targetY<=heroY){ /**start of 7 & 8*/
        if(heroX-targetX>=heroY-targetY){       //7
            int delim = heroY-targetY;
            int allocation[delim];
            for(int z = 0; z<delim;z++)
                allocation[z]=0;
            for(int j =0,i=0;j<heroX-targetX;j++){
                allocation[j%delim]+=1;
                i++;
            }
            tempY--;
            tempX--;
            for(int i=0;i<delim;i++,tempY--){

                if(color==ColorChar::Green)
                    addBulletPath(ColorChar('|'), tempX,tempY);

                for(int j=0;j<allocation[i];j++,tempX--){
                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }

                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('-'), tempX,tempY);

                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                }

            }
            tempY++;
            tempX++;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;
        }
        else if(heroX-targetX<=heroY-targetY){ //8
            int delim = heroX-targetX;
            int allocation[delim];
            for(int z = 0; z<delim;z++)
                allocation[z]=0;
            for(int j =0,i=0;j<heroY-targetY;j++){
                allocation[j%delim]+=1;
                i++;
            }
            tempX--;
            tempY--;
            for(int i=0;i<delim;i++,tempX--){
                if(color==ColorChar::Green)
                    addBulletPath(ColorChar('-'), tempX,tempY);

                for(int j=0;j<allocation[i];j++,tempY--){
                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('|'), tempX,tempY);

                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                }
            }
            tempY++;
            tempX++;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            tileMap[tempY][tempX].setFogFlag(false);
            return;
        }

    }
    else if(targetX>=heroX&&targetY<=heroY){  /** start of 1 & 2*/
        if(targetX-heroX>=heroY-targetY){ //2

            int delim= heroY - targetY;
            int allocation[delim];
            for(int z =0;z<delim;z++)
                allocation[z]=0;
            for(int j=0, i =0;j<targetX-heroX;j++){
                allocation[j%delim]+=1;
                i++;
            }
            tempY--;
            tempX++;
            for(int i =0;i<delim;i++,tempY--){
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('|'), tempX,tempY);

                for(int j=0;j<allocation[i];j++,tempX++){
                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('-'), tempX,tempY);

                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                }
            }
            tempY++;
            tempX--;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;
        }
        else if(targetX-heroX<=heroY-targetY){  //1
            int delim = targetX-heroX;
            int allocation[delim];
            for(int z = 0; z<delim;z++){
                allocation[z]=0;
            }
            for(int j=0, i = 0;j<heroY-targetY;j++){
                allocation[i%delim]+=1;
                i++;
            }
            tempX++;
            tempY--;
            for(int i=0;i<delim;i++,tempX++){
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('-'), tempX,tempY);

                for(int j=0;j<allocation[i];j++,tempY--){
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('|'), tempX,tempY);

                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }
                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                }
            }
            tempY++;
            tempX--;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;

        }
    }
    else if(targetX<=heroX&&targetY>=heroY){ /**start of 5 & 6 */
         if(heroX-targetX>=targetY-heroY){ //6
            int delim = targetY-heroY;
            int allocation[delim];
            for(int z=0;z<delim;z++)
                allocation[z]=0;
            for(int j=0, i=0;j<heroX-targetX;j++){
                allocation[j%delim]+=1;
                i++;
            }
            tempX--;
            tempY++;
            for(int i=0;i<delim;i++,tempY++){
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('|'), tempX,tempY);

                for(int j=0;j<allocation[i];j++,tempX--){
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('-'), tempX,tempY);

                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }
                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                }
            }
            tempY--;
            tempX++;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;
        }

        else if(heroX-targetX<=targetY-heroY){ //5
            int delim = heroX-targetX;
            int allocation[delim];
            for(int z=0;z<delim;z++)
                allocation[z]=0;
            for(int j=0, i =0;j<targetY-heroY;j++){
                allocation[j%delim]+=1;
            }

            tempX--;
            tempY++;
            for(int i =0;i<delim;i++,tempX--){
                if(color==ColorChar::Green)
                    addBulletPath(ColorChar('-'), tempX,tempY);

                for(int j=0;j<allocation[i];j++,tempY++){
                    if(color==ColorChar::Green)
                        addBulletPath(ColorChar('|'), tempX,tempY);

                    if(illegalBulletPath(tempX, tempY)){
                        color = ColorChar::Red;
                    }
                    tileMap[tempY][tempX].setOverlayLvl(ColorChar('*', color));
                    tileMap[tempY][tempX].setFogFlag(false);
                }
            }
            tempX++;
            tempY--;
            tileMap[tempY][tempX].setOverlayLvl(ColorChar('x', color));
            return;

        }
    }
}


/**
Clears the overlay layer for all the tiles on the map.
*/
void TMap::clearOverlay(){
    for(int i=0;i<tileMap.size();i++){
        for(int j=0;j<(tileMap.at(i)).size();j++ ){


            tileMap[i][j].setOverlayLvl('\0');
            tileMap[i][j].revert();
            tileMap[i][j].setFogFlag(false);
        }
    }
}

/**
inserts segments of the bullet path into the tile path
*/
int TMap::showBulletPath(){
    clearOverlay();
    if(trajectory.size()<=0)
        return 0;
    Point p = trajectory.front();
    for(int i=0,j=1;j<trajectory.size();j++,i++){
        trajectory[i]=trajectory[j];
    }
    trajectory.pop_back();
    if(isSpaceOccupied(p.x, p.y)){
        trajectory.clear();

        return getId(p.x,p.y);
    }
    tileMap[p.y][p.x].setOverlayLvl(p.c);
    return -1;
}

/**
checks to see if a bullet can pass over the specified tile. Returns false if the bullet cannot.
*/
bool TMap::illegalBulletPath(int x, int y){
    return tileMap[y][x].getHighest().getChar()=='#' || tileMap[y][x].getHighest().getChar()=='+';
}


/**
Adds a segment to the path of the bullet
*/
void TMap::addBulletPath(ColorChar c, int x, int y){
    trajectory.push_back({c, x, y});
    return;
}


/**
returns the distance between two points
x1 - first x cordinate
y1 - first y cordinate
x2 - second x cordinate
y2 - second y cordinate
*/
int TMap::distance(int x1, int y1, int x2, int y2){
    return abs(x1-x2) + abs(y1-y2);
}

/***
Detects if crosshairs are moved outside of game board
x - x coordinate of the crosshair
y - y coordinate of the crosshair
*/
bool TMap::legalTile(int x, int y){
    return x>=0  && x<tileMap.at(0).size() && y>=0  && y<tileMap.size();
}

/**
returns the highest char at a specified tile
x - x coordinate of a tile
y - y coordinate of a tile
*/
char TMap::getChar(int x, int y){
    return tileMap[y][x].getHighest().getChar();
}

void TMap::setChar(char c, int x, int y, int lvl){
    if(lvl==Tile::overlaylvl){
        tileMap[y][x].setOverlayLvl(c);
        tileMap[y][x].revert();

    }
    else if(lvl==Tile::charlvl){
        tileMap[y][x].setCharLvl(c);
        tileMap[y][x].revert();

    }
    else if(lvl==Tile::droplvl){
        tileMap[y][x].setDropLvl(c);
        tileMap[y][x].revert();
    }
    else if(lvl==Tile::backlvl){
        tileMap[y][x].setBackLvl(c);
        tileMap[y][x].revert();

    }

    return;
}


void TMap::fogOfWar(){
    for(int i=0;i<tileMap.size();i++){
        for(int j=0;j<tileMap.at(i).size();j++ ){
            if(tileMap[i][j].getOverlayLvl().getChar()=='\0')
                tileMap[i][j].setFogFlag(true);
            else
                tileMap[i][j].setOverlayColor(ColorChar::Red);
        }
    }
}

void TMap::look(){
    int max = 4;
    for(int i=0;i<max;i++){
        for(int j=0;j<max-i;j++){
            if(legalTile(getCharXCord(0)+j, getCharYCord(0)-i  )) {
                tileMap[getCharYCord(0)-i][getCharXCord(0)+j].setFogFlag(false);
                tileMap[getCharYCord(0)-i][getCharXCord(0)+j].setFogColor(ColorChar::Gray);
                if(tileMap[getCharYCord(0)-i][getCharXCord(0)+j].getOverlayLvl().getChar()!='\0')
                    tileMap[getCharYCord(0)-i][getCharXCord(0)+j].setOverlayColor(ColorChar::Green);
            }

            if(legalTile(getCharXCord(0)-j, getCharYCord(0)-i )) {
                tileMap[getCharYCord(0)-i][getCharXCord(0)-j].setFogFlag(false);
                tileMap[getCharYCord(0)-i][getCharXCord(0)-j].setFogColor(ColorChar::Gray);
                if(tileMap[getCharYCord(0)-i][getCharXCord(0)-j].getOverlayLvl().getChar()!='\0')
                    tileMap[getCharYCord(0)-i][getCharXCord(0)-j].setOverlayColor(ColorChar::Green);
            }
            if(legalTile(j+getCharXCord(0), i+getCharYCord(0) )) {
                tileMap[getCharYCord(0)+i][getCharXCord(0)+j].setFogFlag(false);
                tileMap[getCharYCord(0)+i][getCharXCord(0)+j].setFogColor(ColorChar::Gray);
                if(tileMap[getCharYCord(0)+i][getCharXCord(0)+j].getOverlayLvl().getChar()!='\0')
                    tileMap[getCharYCord(0)+i][getCharXCord(0)+j].setOverlayColor(ColorChar::Green);
            }

            if(legalTile(getCharXCord(0)-j, getCharYCord(0)+i )) {
                tileMap[getCharYCord(0)+i][getCharXCord(0)-j].setFogFlag(false);
                tileMap[getCharYCord(0)+i][getCharXCord(0)-j].setFogColor(ColorChar::Gray);
                if(tileMap[getCharYCord(0)+i][getCharXCord(0)-j].getOverlayLvl().getChar()!='\0')
                    tileMap[getCharYCord(0)+i][getCharXCord(0)-j].setOverlayColor(ColorChar::Green);
            }
        }
    }
}

ColorChar TMap::getDropLvl(int x, int y){
    return tileMap[y][x].getDropLvl();
}
