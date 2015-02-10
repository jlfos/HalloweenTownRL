struct Tile {
    bool explored; // has the player already seen this tile ?
    Tile() : explored(false) {}
};
 
class Map : public Persistent{
public :
    int width,height;
 
    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
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
private:
    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);
    friend class BspListener;
};
