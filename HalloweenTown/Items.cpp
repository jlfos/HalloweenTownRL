#include <Items.h>
Items::Items(){
}

int Items::getClipSize(string s){
   if(s=="pistol")
    {
        return pistolClipSize;
    }
   else if(s=="shotgun")
    {
        return shotgunClipSize;
    }
}


int Items::getDieFace(string s){
    if(s=="pistol")
    {
        return pistolDieFace;
    }
    else if(s=="shotgun")
    {
        return shotgunDieFace;
    }
}

int Items::getDieNumber(string s){
    if(s=="pistol")
    {
        return pistolDieNumber;
    }
    else if(s=="shotgun")
    {
        return shotgunDieNumber;
    }
}
