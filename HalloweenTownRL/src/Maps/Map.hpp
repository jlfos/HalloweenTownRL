#ifndef HALLOWEENTOWN_MAP_HPP
#define HALLOWEENTOWN_MAP_HPP
#include <vector>
#include "../Tile/TileCharacters.hpp"
#include "../Actor/ActorFactory.hpp"
#include "../Persistent.hpp"
#include "Point.hpp"

class Time;
class MapGenerator;
class Actor;

struct Tile {
    Tile() : explored(false), character(TileCharacters::Default::RAINBOW), lit(false), visibility(0) {}
    TCODColor visibleColor;
    TCODColor fogColor;
    bool explored; // has the player already seen this tile ?
    int character; //character representing the tile
    bool lit; //is the tile currently lit?
    float visibility;

};

class Map : public Persistent{
private:
    bool NotOnMap(int x, int y) const;
    int width,height;
    MapGenerator* generator;
    Time* lastSeen;

    long seed;
    TCODRandom *rng;
    TCODMap *map;
    int a = 0;
public :
    Map(int width, int height);
    Map(int width, int height, MapGenerator* generator);
    ~Map();
    TCODList<Actor*> actors;
    enum class TileType {GROUND, WALL, LEFT_EDGE, RIGHT_EDGE, TOP_EDGE, BOTTOM_EDGE};
    TileType GetTileType(int x, int y) const;
	bool IsInFov(int x, int y);
    bool IsExplored(int x, int y) const;
    bool CanWalk(int x, int y) const;
    void ComputeFov();
    void Render();
    void Init();
    void Load(TCODZip &zip);
    void Save(TCODZip &zip);
    void PopulateActors();
    Time* TimeLastSeen();
    void TimeLastSeen(Time* time);
    int GetWidth();
    int GetHeight();
    void SetTileProperties(int tileIndex, TCODColor visible, int character);
    void SetTileProperties(Point point, TCODColor visible, int character);
    bool TileHasBeenSet(int tileIndex);
    bool TileHasBeenSet(Point point);
    std::vector<Point> spawnLocations;
    ActorFactory::EnemyDifficulty GetDifficulty();
    std::vector<Tile> tiles;
    void computeLight(Actor *owner, bool isVisible);
    void computeLight(Actor *owner, bool isVisible, int radius);
    void computeNonplayerLights();
    float getTileVisibility(int x, int y);
};
#endif
