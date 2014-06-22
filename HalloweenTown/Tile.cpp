#include "Driver.hpp"

Tile::Tile(){
    highest =99;
    fogFlag = true;
    overlayLvl=ColorChar('\0', 7);
    charLvl=ColorChar('\0', 7);
    dropLvl=ColorChar('\0', 7);
    backLvl=ColorChar('.', 7);

}

Tile::Tile(ColorChar c, int level)
{   fogFlag = true;
    if (level==0)
    {
        overlayLvl=c;
        charLvl=ColorChar('\0', 7);
        dropLvl=ColorChar('\0', 7);
        backLvl=ColorChar('.', 7);
    }
    else if (level==1)
    {
        charLvl=c;
        dropLvl=ColorChar('\0', 7);
        backLvl=ColorChar('.', 7);
    }
    else if (level==2)
    {
        charLvl=ColorChar('\0', 7);
        dropLvl=c;
        backLvl=ColorChar('\0', 7);
    }
    else if (level==3)
    {
        charLvl=ColorChar('\0', 7);
        dropLvl=ColorChar('\0', 7);
        backLvl=c;
    }
    highest=level;
}

void Tile::setOverlayLvl(char c)
{
    overlayLvl=ColorChar(c,7);
    highest = 0;
}

void Tile::setOverlayLvl(ColorChar c)
{
    overlayLvl=c;
    highest = 0;
}


/**
Inserts a ColorChar at the character level of the tile.
*/
void Tile::setCharLvl(ColorChar c)
{

    charLvl=c;

    highest=1;

}

/**
Inserts a ColorChar at the character level of the tile(Defaulted to white color).
*/
void Tile::setCharLvl(char c)
{


    charLvl=ColorChar(c,7);
    highest=1;

}

/**
Inserts a char at the drop level of the tile.
*/
void Tile::setDropLvl(ColorChar c)
{


    dropLvl=c;
    highest=2;
}

/**
Inserts a ColorChar at the drop level of the tile(Defaulted to white color).
*/
void Tile::setDropLvl(char c)
{
    dropLvl=ColorChar(c,7);
    highest=2;
}


/**
Inserts a char at the back level of the tile.
*/
void Tile::setBackLvl(ColorChar c)
{
    backLvl=c;
    highest=3;
}

/**
Inserts a ColorChar at the back level of the tile(Defaulted to white color).
*/
void Tile::setBackLvl(char c)
{
    backLvl.setChar(c);
    highest=3;
}



/**
Inserts a char for the view of the tile.
*/
void Tile::setFog(ColorChar c)
{
    fog=c;
}

/**
Inserts a ColorChar for the view of the tile(Defaulted to black color).
*/
void Tile::setFog(char c)
{
    fog= ColorChar(c, ColorChar::Black);
}

void Tile::setFogFlag(bool b)
{
    fogFlag=b;
}

void Tile::setFogColor(int i){
    fog.setColor(i);
}

/**
Retruns the top most char of the tile
*/
ColorChar Tile::getHighest()
{



    if (fogFlag)
        return fog;
    else
    {
        if (highest==0)
            return overlayLvl;
        else if (highest==1){

            return charLvl;
        }
        else if (highest==2)
            return dropLvl;
        else if (highest==3)
            return backLvl;
    }
}

/**
Returns the tile back to it's previous highest.
*/
void Tile::revert()
{
    if (highest==3)
    {
        return;
    }
    else
    {
        bool temp = fogFlag;
        fogFlag=false;
        for(highest=0;highest<4;highest++){


            if ((getHighest()).getChar()!='\0')
                return;
        }
        fogFlag=temp;
       // bool temp = fogFlag;
       // fogFlag=false;
       /* if ((getHighest()).getChar()=='\0'){

            revert();
        }//*/
    }
}

ColorChar Tile::getCharLvl()
{
    return charLvl;
}

ColorChar Tile::getOverlayLvl()
{
    return overlayLvl;
}

void Tile::setOverlayColor(int i){
    if(overlayLvl.getChar()=='|'||overlayLvl.getChar()=='\\'||overlayLvl.getChar()=='/'||overlayLvl.getChar()=='-')
        return;
    overlayLvl.setColor(i);
}

int Tile::blah(){
    return highest;
}

ColorChar Tile::getDropLvl(){
    return dropLvl;
}
