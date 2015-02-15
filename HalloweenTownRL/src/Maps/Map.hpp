struct Tile {
    bool explored; // has the player already seen this tile ?
    Tile() : explored(false) {}
};
 
class Map : public Persistent{
private:

    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);
    friend class BspListener;
    bool notOnMap(int x, int y) const;
public :
    int width,height;
 
    Map(int width, int height);
    ~Map();
    enum class TileType {GROUND, WALL, LEFT_EDGE, RIGHT_EDGE, TOP_EDGE, BOTTOM_EDGE};
    TileType getTileType(int x, int y) const;
	bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    bool canWalk(int x, int y) const;
    void computeFov();
    void render() const;
    void addItem(int x, int y);
    void init(bool withActors);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
protected :
    Tile *tiles;
    TCODMap *map;
    long seed;
    TCODRandom *rng;
    void addMonster(int x, int y);
};
