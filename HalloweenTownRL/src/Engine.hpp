#include <vector>

using namespace std;

class Engine {

private:

public :
	enum GameStatus{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	} gameStatus;

    TCODList<Actor*> actors;
    Actor *player;
    Map *currentMap;
    vector<vector<Map*>> *maps;
    int fovRadius;
    int screenWidth;
    int screenHeight;
    Gui *gui;
    TCOD_key_t lastKey;
    TCOD_mouse_t mouse;

    Engine(int screenWidth, int screenHeight);
    ~Engine();
    void update();
    void render();
    void init();
    void term();
    void load();
    void save();
    void sendToBack(Actor *actor);
    void nextLevel(Map::TileType type);
private:
	void exitGame();
	void newGame();
	void continueGame();
	const int DEFAULT_MAP_WIDTH = 80;
	const int DEFAULT_MAP_HEIGHT = 43;
	const int DEFAULT_PLAYER_START_X = 40;
	const int DEFAULT_PLAYER_START_Y = 25;
	const int DEFAULT_MAP_X = 1;
	const int DEFAULT_MAP_Y = 1;
	const int WORLD_SIZE_LATITUDE = 3;
	const int WORLD_SIZE_LONGITUDE = 3;
	vector<vector<Map*>> *CreateMaps();
	int mapX;
	int mapY;
};
 


extern Engine engine;
