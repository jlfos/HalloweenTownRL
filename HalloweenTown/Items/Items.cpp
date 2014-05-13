#include "Items.h"
Items::Items(){
}

int Items::ZgetClipSize(string s){
   if(s=="pistol")
    {
        return pistolClipSize;
    }
   else if(s=="shotgun")
    {
        return shotgunClipSize;
    }
}


int Items::ZgetDieFace(string s){
    if(s=="pistol")
    {
        return pistolDieFace;
    }
    else if(s=="shotgun")
    {
        return shotgunDieFace;
    }
}

int Items::ZgetDieNumber(string s){
    if(s=="pistol")
    {
        return pistolDieNumber;
    }
    else if(s=="shotgun")
    {
        return shotgunDieNumber;
    }
}
