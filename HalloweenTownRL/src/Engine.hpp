#ifndef HALLOWEENTOWN_ENGINE_HPP
#define HALLOWEENTOWN_ENGINE_HPP
#include <vector>
#include <set>
#include "libtcod.hpp"
#include "Maps/Map.hpp"
#include "MonsterEntry.hpp"
#include "Time.hpp"

class Actor;
class Gui;

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
    std::vector<std::vector<Map*>> *maps;
    int fovRadius;
    int screenWidth;
    int screenHeight;
    Gui *gui;
    TCOD_key_t lastKey;
    TCOD_mouse_t mouse;

    Engine(int screenWidth, int screenHeight);
    ~Engine();
    void Update();
    void Render();
    void Init();
    void Term();
    void Load();
    void Save();
    void SendToBack(Actor *actor);
    void NextLevel(Map::TileType type);
	Time currentTime;
private:
	enum class MapType {FOREST_NORTH, FOREST_SOUTH, ROAD_EW, CITY };
	void ExitGame();
	void NewGame();
	void ContinueGame();
	const int DEFAULT_MAP_WIDTH = 80;
	const int DEFAULT_MAP_HEIGHT = 43;
	const int DEFAULT_PLAYER_START_X = 5;
	const int DEFAULT_PLAYER_START_Y = 20;
	const int DEFAULT_MAP_X = 1;
	const int DEFAULT_MAP_Y = 1;
	int WORLD_SIZE_LATITUDE;
	int WORLD_SIZE_LONGITUDE;
	std::vector<std::vector<Map*>> *CreateMaps(std::vector<std::vector<Engine::MapType>>);
	int mapX;
	int mapY;
	bool incrementTime;
//	std::set<MonsterEntry, MonsterEntry::MonsterEntry_compare> monsterLog;
};
 


extern Engine engine;
#endif
