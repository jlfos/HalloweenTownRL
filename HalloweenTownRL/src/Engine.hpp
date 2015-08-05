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
	std::string getItemDescription(std::string name);
private:
	enum class MapType {FOREST_NORTH, FOREST_SOUTH, ROAD_EW, CITY, CITY_BOSS };
	void ExitGame();
	void NewGame();
	void ContinueGame();
	bool incrementTime;
	int bossMapX;
	int bossMapY;
	int playerMapX;
	int playerMapY;
	int WORLD_SIZE_LATITUDE;
	int WORLD_SIZE_LONGITUDE;

	std::vector<std::vector<Map*>> *CreateMaps(std::vector<std::vector<Engine::MapType>>);
	void bossUpdate();


//	std::set<MonsterEntry, MonsterEntry::MonsterEntry_compare> monsterLog;
};
 


extern Engine engine;
#endif
