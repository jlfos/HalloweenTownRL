#include <vector>
#include <time.h>
#include "Action.h"
#include "Actor.h"
#include "TMap.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Monster : public Actor{
    private:
        int hate;
        vector<int> moveList;
        vector< vector< int> > masterMoveList;
        string name;
        void simulatePath(int x1, int y1, int x2, int y2, int limit, TMap m, vector<int> v);
        vector<int> bestPath();
        //int reference;
    public:
        Monster(ColorChar c, int i): Actor(c, i) {}
        Action nextAction(TMap m);
        string getName();
        void lowerReference();
};
