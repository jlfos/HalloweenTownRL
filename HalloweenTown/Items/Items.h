#include <string>
#pragma once
using namespace std;
class Items{

    private:
        //Pistol stats
        const static int pistolDieFace = 6;
        const static int pistolDieNumber = 1 ;
        const static int pistolClipSize = 9;

        //Shotgun stats
        const static int shotgunDieFace = 8;
        const static int shotgunDieNumber = 2 ;
        const static int shotgunClipSize = 4;


    public:
        typedef enum ItemType {
        	FIREARM
        };
        Items();
        int ZgetClipSize(string s);
        int ZgetDieFace(string s);
        int ZgetDieNumber(string s);

};
