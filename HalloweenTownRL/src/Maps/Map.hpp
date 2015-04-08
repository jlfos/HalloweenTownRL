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
    bool NotOnMap(int x, int y) const;
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
    TileType GetTileType(int x, int y) const;
	bool IsInFov(int x, int y) const;
    bool IsExplored(int x, int y) const;
    bool CanWalk(int x, int y) const;
    void ComputeFov();
    void Render() const;
    void Init();
    void Load(TCODZip &zip);
    void Save(TCODZip &zip);
    void PopulateActors();
    Time* TimeLastSeen();
    void TimeLastSeen(Time* time);
    vector<Point> spawnLocations;
    ActorFactory::EnemyDifficulty GetDifficulty();
protected :
    TCODMap *map;
    long seed;
    TCODRandom *rng;
};
