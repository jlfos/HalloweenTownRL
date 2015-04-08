#include <iostream>
#include <chrono>
#include <vector>
#include <time.h>
#include "libtcod.hpp"
#include "main.hpp"

using namespace std;

Engine::Engine(int screenWidth, int screenHeight) :
		gameStatus(STARTUP), fovRadius(60), screenWidth(screenWidth), screenHeight(screenHeight),
		currentTime(6, 00), incrementTime(false){
	try {
		currentMap = nullptr;
		maps = nullptr;
		player = nullptr;

		TCODConsole::initRoot(screenWidth, screenHeight, "HalloweenTown",
				false);

		gui = new Gui();

	} catch (...) {
		cerr << "An error occurred with Engine::Engine" << endl;
		throw 0;
	}
}

void Engine::Init() {
	try {

		player = ActorFactory::CreateHero(DEFAULT_PLAYER_START_X,
				DEFAULT_PLAYER_START_Y);
		maps = CreateMaps();
		mapX = DEFAULT_MAP_X;
		mapY = DEFAULT_MAP_Y;

		currentMap = (*maps)[mapX][mapY];
		actors = currentMap->actors;
		actors.push(player);
		gui->PushMessage(TCODColor::red,
				"Welcome stranger!\nPrepare to perish in the horrors of Halloween Town.");
		gameStatus = STARTUP;
	} catch (...) {
		cerr << "An error occurred with Engine::Init" << endl;
		throw 0;
	}
}

vector<vector<Map*>> *Engine::CreateMaps() {
	try {
		vector<vector<Map*>> *maps = new vector<vector<Map*>>(
				WORLD_SIZE_LATITUDE);

		TCODRandom *rng = TCODRandom::getInstance();
		for (int i = 0; i < WORLD_SIZE_LATITUDE; i++) {
			for (int j = 0; j < WORLD_SIZE_LONGITUDE; j++) {
				 if (i  == 1) {
					Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT,
							new RoadMapGenerator());
					if (temp) {
						maps->at(i).push_back(temp);
					}
				} else {
					Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT,
							new CityMapGenerator());
					if (temp) {
						maps->at(i).push_back(temp);
					}
				}
				(*maps)[i][j]->Init();

			}
		}

		return maps;
	} catch (...) {
		cerr << "An error occurred in Engine::CreateMaps" << endl;
	}
}

void Engine::Term() {

	try {

		actors.clearAndDelete();
		if (maps != nullptr) {
			for (int i = 0; i < WORLD_SIZE_LATITUDE; i++) {
				for (int j = 0; j < WORLD_SIZE_LONGITUDE; j++) {

					delete (*maps)[i][j];
				}
			}
			delete maps;
			maps = nullptr;
		}
		gui->Clear();
	} catch (...) {
		cerr << "An error occurred with Engine::Term" << endl;
		throw 0;
	}
}

void Engine::Save() {
	try {
		if (player->destructible->IsDead()) {
			TCODSystem::deleteFile("game.sav");
		} else {
			TCODZip zip;
			//save the map first
			zip.putInt(currentMap->width);
			zip.putInt(currentMap->height);
			zip.putInt(mapX);
			zip.putInt(mapY);
			zip.putInt(WORLD_SIZE_LATITUDE);
			zip.putInt(WORLD_SIZE_LONGITUDE);
			player->Save(zip);

			actors.remove(player);

			for (int i = 0; i < WORLD_SIZE_LATITUDE; i++) {
				for (int j = 0; j < WORLD_SIZE_LONGITUDE; j++) {
					(*maps)[i][j]->Save(zip);
				}
			}

			gui->Save(zip);
			zip.saveToFile("game.sav");
		}
	} catch (...) {
		cerr << "An error occurred with Engine::Save" << endl;
		throw 0;
	}
}

void Engine::Load() {
	try {
		bool saveGameExists;
		if (TCODSystem::fileExists("game.sav")) {
			saveGameExists = true;

		}
		engine.gui->menu.PopulateMenu(saveGameExists);

		string menuItem = engine.gui->menu.Pick();

		if (menuItem == "Exit" || menuItem == "NONE") {
			ExitGame();
		}
		else if (menuItem == "New Game") {
			NewGame();
			gameStatus = STARTUP;
		}
		else if (menuItem == "Continue") {
			ContinueGame();
		}
	} catch (...) {
		cerr << "An error occurred with Engine::load" << endl;
		throw 0;
	}
}

void Engine::ExitGame() {
	try {
		Save();
		exit(0);

	} catch (...) {
		cerr << "An error occurred with Engine::exitGame" << endl;
		throw 0;
	}
}

void Engine::NewGame() {
	try {
		Term();
		Init();
	} catch (...) {
		cerr << "An error occurred with Engine::newGame" << endl;
		throw 0;
	}
}

void Engine::ContinueGame() {
	try {
		if(gameStatus == STARTUP){
			cout << "Load game from save" << endl;
			Term();
			TCODZip zip;
			zip.loadFromFile("game.sav");

			int width = zip.getInt();
			int height = zip.getInt();
			mapX = zip.getInt();
			mapY = zip.getInt();
			int tempLatitudeSize = zip.getInt();
			int tempLongitudeSize = zip.getInt();
			if (tempLatitudeSize != WORLD_SIZE_LATITUDE
					|| tempLongitudeSize != WORLD_SIZE_LONGITUDE) {
				cerr << "An error occurred with loading the save file." << endl;
				return;
			}

			player = new Actor(0, 0, 0, nullptr, TCODColor::white);
			player->Load(zip);

			maps = new vector<vector<Map*>>(WORLD_SIZE_LATITUDE);
			for (int i = 0; i < tempLatitudeSize; i++) {
				for (int j = 0; j < tempLongitudeSize; j++) {
					(*maps)[i].push_back(new Map(width, height));
					(*maps)[i][j]->Load(zip);
					if (mapX == i && mapY == j) {
						currentMap = (*maps)[i][j];
						actors = (*maps)[i][j]->actors;
						actors.push(player);
					}
				}
			}

			gui->Load(zip);

			gameStatus = STARTUP;
		}
		else{
			cout << "Return to game" << endl;
			engine.gui->menu.Clear();

		}
	} catch (...) {
		cerr << "An error occurred with Engine::ContinueGame" << endl;
		throw 0;
	}
}

Engine::~Engine() {
	try {
		if (gui != nullptr) {
			delete gui;
			gui = nullptr;
		}

		for (int i = 0; i < WORLD_SIZE_LATITUDE; i++) {
			for (int j = 0; j < WORLD_SIZE_LONGITUDE; j++) {
				if (i == mapX && j == mapY && player != nullptr) {
					(*maps)[i][j]->actors.remove(player);
				}
				delete (*maps)[i][j];
			}
		}

		if (maps != nullptr) {
			delete maps;
			maps = nullptr;
		}

	} catch (...) {
		cerr << "An error occurred with Engine::~Engine" << endl;
		throw 0;
	}
}

void Engine::Update() {
	try {
		if (gameStatus == STARTUP)
			currentMap->ComputeFov();
		gameStatus = IDLE;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
		player->Update();
		if (gameStatus == NEW_TURN) {
			if(incrementTime)
				currentTime.IncrementMinutes();
			incrementTime = !incrementTime;
			for (Actor *actor : actors) {
				if (actor != player) {
					actor->Update();
				}
			}
		}
	} catch (...) {
		cerr << "An error occurred with Engine::Update" << endl;
		throw 0;
	}
}


void Engine::NextLevel(Map::TileType type) {
	try {
		int heroX = DEFAULT_PLAYER_START_X;
		int heroY = DEFAULT_PLAYER_START_Y;

		if (type == Map::TileType::TOP_EDGE) {
			heroX = player->x;
			heroY = DEFAULT_MAP_HEIGHT - 1;
			if (mapY < WORLD_SIZE_LATITUDE - 1) {
				mapY++;
			} else {
				gui->PushMessage(TCODColor::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::RIGHT_EDGE) {
			heroX = 0;
			heroY = player->y;
			if (mapX < WORLD_SIZE_LONGITUDE - 1) {
				mapX++;
			} else {
				gui->PushMessage(TCODColor::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::BOTTOM_EDGE) {
			heroX = player->x;
			heroY = 0;
			if (mapY > 0) {
				mapY--;
			} else {
				gui->PushMessage(TCODColor::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::LEFT_EDGE) {
			heroX = DEFAULT_MAP_WIDTH - 1;
			heroY = player->y;
			if (mapX > 0) {
				mapX--;
			} else {
				gui->PushMessage(TCODColor::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		actors.remove(player);
		player = ActorFactory::CreateHero(heroX, heroY);
		currentMap = (*maps)[mapX][mapY];
		bool populateFlag = false;
		if(currentMap->TimeLastSeen()){
			if(currentTime.ElapsedMinutes((*currentMap->TimeLastSeen())) >= 60)
				populateFlag = true;
		}
		else{
			currentMap->TimeLastSeen(new Time(currentTime.GetHour(), currentTime.GetMinutes()));
			populateFlag = true;
		}

		if(populateFlag)
			currentMap->PopulateActors();
		actors = currentMap->actors;
		actors.push(player);
		gameStatus = STARTUP;
		Update();
	} catch (...) {
		cerr << "An error occurred with Engine::nextLevel" << endl;
		throw 0;
	}
}

void Engine::SendToBack(Actor *actor) {
	try {
		actors.remove(actor);
		actors.insertBefore(actor, 0);
	} catch (...) {
		cerr << "An error occurred with Engine::sendToBack" << endl;
		throw 0;
	}
}

void Engine::Render() {
	try {
		TCODConsole::root->clear();
		// draw the map
		currentMap->Render();

		gui->Render();

		TCODConsole::root->print(1, screenHeight - 2, "HP: %d/%d",
				(int) player->destructible->hp,
				(int) player->destructible->maxHp);

		// draw the actors
		for (Actor *actor : actors) {
			if (currentMap->IsInFov(actor->x, actor->y)) {
				actor->Render();
			}
		}
		TCODConsole::flush();

	}
	catch (...) {
		cerr << "An error occurred with Engine::Render" << endl;
		throw 0;
	}
}
