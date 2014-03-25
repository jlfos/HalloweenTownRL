#pragma once
#include "ColorChar.h"
class Tile{
    private:
     ColorChar fog;
     bool fogFlag;
     ColorChar overlayLvl; //0
     ColorChar charLvl; // 1
     ColorChar dropLvl; // 2
     ColorChar backLvl; // 3
     int highest; //current highest char for this tile
    public:
        Tile();
        enum level{
            overlaylvl,
            charlvl,
            droplvl,
            backlvl
        };
        Tile(ColorChar d, int level); //
        ColorChar getHighest();
        void setOverlayLvl(ColorChar c);
        void setCharLvl(ColorChar c);
        void setDropLvl(ColorChar c);
        void setBackLvl(ColorChar c);
        void setFog(ColorChar c);
        void setFog(char c);
        void setFogFlag(bool b);
        int blah();
        void setFogColor(int i);
        void setCharLvl(char c);
        void setOverlayLvl(char c);
        void setDropLvl(char c);
        void setBackLvl(char c);
        void revert();
        void setOverlayColor(int i);
        ColorChar getDropLvl();
        ColorChar getCharLvl();
        ColorChar getOverlayLvl();
};
