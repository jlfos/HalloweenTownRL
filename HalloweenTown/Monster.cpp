#include "Monster.h"

Action Monster::nextAction(TMap m){
    masterMoveList.clear();
    int dir =1;
    dir += rand()%4;
    hate = 0;
    map<string,int> params;


    simulatePath(m.getCharXCord(reference), m.getCharYCord(reference), m.getCharXCord(0), m.getCharYCord(0), 15, m, vector<int>() );
    if(bestPath().size()>0){
         dir= bestPath().front();
    }
    if(dir==TMap::Up){
        if(m.getCharXCord(reference)==m.getCharXCord(0)&&m.getCharYCord(reference)-1==m.getCharYCord(0)){
            params["dam"]=5;
            params["cha"] = 5;
            params["tar"] = 0;
            return Action(1, params);
        }

    }
    if(dir==TMap::Down){
        if(m.getCharXCord(reference)==m.getCharXCord(0)&&m.getCharYCord(reference)+1==m.getCharYCord(0)){
            params["dam"]=5;
            params["cha"] = 5;
            params["tar"] = 0;
            return Action(1, params);
        }

    }
    if(dir==TMap::Left){
        if(m.getCharXCord(reference)-1==m.getCharXCord(0)&&m.getCharYCord(reference)==m.getCharYCord(0)){
            params["dam"]=5;
            params["cha"] = 5;
            params["tar"] = 0;
            return Action(1, params);
        }

    }
    if(dir==TMap::Right){
        if(m.getCharXCord(reference)+1==m.getCharXCord(0)&&m.getCharYCord(reference)==m.getCharYCord(0)){
            params["dam"]=5;
            params["cha"] = 5;
            params["tar"] = 0;
            return Action(1, params);
        }

    }


    params.insert(pair<string,int> ("dir", dir) );

   return Action(0, params);
}

string Monster::getName(){
    return "goblin";
}

void Monster::lowerReference(){
    reference--;
    return;
}

/*vector<int> Monster::findPath(int x1, int y1, int x2, int y2){
    vector<int> shortest();
    vector<int> current();
    int tempX=x1;
    int tempY=y1;
    int dir =1;
    dir += (rand()%4);
    while(){
        bool direction = true;
        while(direction){
            if(m.legalMove(tempX, tempY, TMap::Up)){
                    current.push_back(dir);
                    tempY--;

            }
            else if(m.legalMove(tempX, tempY, TMap::Down)){
                    current.push_back(dir);
                    tempY++;
            }
            else if(m.legalMove(tempX, tempY, TMap::Right)){
                    current.push_back(dir);
                    tempX++;
            }
            if(m.legalMove(tempX, tempY, TMap::Up)){
                    current.push_back(dir);
                    tempY--;
            }



        }
    }
}//*/

void Monster::simulatePath(int x1, int y1, int x2, int y2, int limit, TMap m, vector<int> v){

    int targetDist = m.distance(x1, y1, x2, y2);


        /*

        mvprintw(21,20,"|oldD: %i |newD: %i |2comp: %i",targetDist,m.distance(x1, 1+y1, x2, y2) , (targetDist > m.distance(x1, 1+y1, x2, y2))   );// mvprintw(20,20,"L:%i X:%i Y%i", limit, x1, y1);
                mvprintw(22,20,"|1comp: %i |totalComp: %i",
                 m.legalMove(x1, y1, TMap::Down), m.legalMove(x1, y1, TMap::Down && targetDist > m.distance(x1, 1+y1, x2, y2)) );
                refresh();
                getch();

        ///*/

    if(limit==0){
        return;
    }

    if(x1==x2&&y1==y2){
        masterMoveList.push_back(v);
        return ;
    }

        if(x1==x2&&y1-1==y2){

            v.push_back(TMap::Up);
            masterMoveList.push_back(v);
            return;
        }
        if(x1==x2&&y1+1==y2){
            v.push_back(TMap::Down);
            masterMoveList.push_back(v);
            return;
        }
        if(x1-1==x2&&y1==y2){
            v.push_back(TMap::Left);
            masterMoveList.push_back(v);
            return;
        }
        if(x1+1==x2&&y1==y2){
            v.push_back(TMap::Right);
            masterMoveList.push_back(v);
            return;
        }


            if(m.legalMove(x1, y1, TMap::Up) &&  targetDist > m.distance(x1, -1+y1, x2, y2) ){
                // v.push_back( (rand()%4)+1);

                    v.push_back(TMap::Up);
                    int temp=y2+1;
                    simulatePath(x1, y1-1, x2, y2, limit-1, m,v);
                    v.pop_back();
            }
            if( m.legalMove(x1, y1, TMap::Down) && (targetDist > m.distance(x1, 1+y1, x2, y2) ) ) {


                    v.push_back(TMap::Down);
                    simulatePath(x1, y1+1, x2, y2, limit-1, m, v);
                    v.pop_back();
            }

            if( m.legalMove(x1, y1, TMap::Right)  && (targetDist > m.distance(1+x1, y1, x2, y2)) ) {
                    v.push_back(TMap::Right);
                    simulatePath(x1+1, y1, x2, y2, limit-1, m, v);
                    v.pop_back();
            }
            if( m.legalMove(x1, y1, TMap::Left) &&  (targetDist > m.distance(-1+x1, y1, x2, y2)) ){
                    v.push_back(TMap::Left);
                    simulatePath(x1-1, y1, x2, y2, limit-1, m, v);
                    v.pop_back();
            }

          //  v.push_back( (rand()%4)+1);
       // masterMoveList.push_back(v);

            return ;
}

vector<int> Monster::bestPath(){
    int smallestSize=1000000;
    int smallestIndex;
    if(masterMoveList.size()==0)
        return vector<int>();
    for(int i=0;i<masterMoveList.size();i++){
           if( masterMoveList.at(i).size()<smallestSize && masterMoveList.at(i).size()>0 ){
            smallestSize= masterMoveList.at(i).size();
            smallestIndex=i;
           }

    }
    return masterMoveList.at(smallestIndex);

}
