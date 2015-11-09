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
public:
    std::vector<Tile> tiles;
	TCODList<Actor*> actors;
    std::vector<Point> spawnLocations;

private:
    u_int width,height;
    Time* lastSeen;
    MapGenerator* generator;

    TCODRandom *rng;
    TCODMap *map;
    long seed;

public :
    Map(int width, int height, MapGenerator* generator);
    ~Map();

    enum class TileType {GROUND, WALL, LEFT_EDGE, RIGHT_EDGE, TOP_EDGE, BOTTOM_EDGE};
    TileType GetTileType(int x, int y) const;
	bool IsInFov(int x, int y);

    bool CanWalk(int x, int y) const;
    void ComputeFov();
    void Render();
    void PopulateActors();
    void Load(TCODZip &zip);
    void Save(TCODZip &zip);

    Time* TimeLastSeen();
    void TimeLastSeen(Time* time);
    int GetWidth() const;
    bool TileSetOnLineXAxis(const Point start, const Point end);
    bool TileSetOnLineYAxis(const Point start, const Point end);
    int GetHeight() const;
    void SetTileProperties(int x, int y, TCODColor visible, int character);
    void SetTileProperties(Point point, TCODColor visible, int character);
    bool TileHasBeenSet(int x, int y) const;
    bool TileHasBeenSet(Point point) const;

    ActorFactory::EnemyDifficulty GetDifficulty();
    void ComputeLight(Actor *owner, bool isVisible);
    void ComputeAllLights();
    float GetTileVisibility(int x, int y);
    int GetCharacter(int x, int y);
private:
    bool IsExplored(int x, int y) const;
    bool ValidPoint(u_int x, u_int y) const;
};
#endif
