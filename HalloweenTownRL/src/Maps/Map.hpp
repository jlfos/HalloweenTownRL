#include <vector>

class Time;

using namespace std;

struct Tile {
    bool explored; // has the player already seen this tile ?
    Tile() : explored(false) {}
    TCODColor visibleColor;
    TCODColor fogColor;
    int character;
};

struct Point {
	int x;
	int y;
};
class Map : public Persistent{
private:
    bool notOnMap(int x, int y) const;
    MapGenerator* generator;
    Time* lastSeen;
public :
    int width,height;
    Tile *tiles;
    Map(int width, int height);
    Map(int width, int height, MapGenerator* generator);
    ~Map();
    TCODList<Actor*> actors;
    enum class TileType {GROUND, WALL, LEFT_EDGE, RIGHT_EDGE, TOP_EDGE, BOTTOM_EDGE};
    TileType getTileType(int x, int y) const;
	bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    bool canWalk(int x, int y) const;
    void computeFov();
    void render() const;
    void init();
    void load(TCODZip &zip);
    void save(TCODZip &zip);
    void populateActors();
    Time* getTimeLastSeen();
    void setTimeLastSeen(Time* time);
    vector<Point> spawnLocations;
    ActorFactory::EnemyDifficulty GetDifficulty();
protected :
    TCODMap *map;
    long seed;
    TCODRandom *rng;
};
