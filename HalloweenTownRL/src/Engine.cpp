#include <iostream>
#include <vector>
#include "libtcod.hpp"
#include "main.hpp"

using namespace std;

Engine::Engine(int screenWidth, int screenHeight) :
		gameStatus(STARTUP), fovRadius(50), screenWidth(screenWidth), screenHeight(screenHeight) {
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

void Engine::init() {
	try {

		player = ActorFactory::CreateHero(DEFAULT_PLAYER_START_X,
				DEFAULT_PLAYER_START_Y);
		maps = CreateMaps();
		mapX = DEFAULT_MAP_X;
		mapY = DEFAULT_MAP_Y;

		currentMap = (*maps)[mapX][mapY];
		actors = currentMap->actors;
		actors.push(player);
		gui->message(TCODColor::red,
				"Welcome stranger!\nPrepare to perish in the horrors of Halloween Town.");
		gameStatus = STARTUP;
	} catch (...) {
		cerr << "An error occurred with Engine::init" << endl;
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

				int rand = rng->getInt(0, 100);
				if (rand % 2 == 0) {
					Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT,
							new EmptyMapGenerator());
					if (temp) {
						maps->at(i).push_back(temp);
					}
				} else {
					Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT,
							new EmptyMapGenerator());
					if (temp) {
						maps->at(i).push_back(temp);
					}
				}
				(*maps)[i][j]->init();

			}
		}

		return maps;
	} catch (...) {
		cerr << "An error occurred in Engine::CreateMaps" << endl;
	}
}

void Engine::term() {

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
		gui->clear();
	} catch (...) {
		cerr << "An error occurred with Engine::term" << endl;
		throw 0;
	}
}

void Engine::save() {
	try {
		if (player->destructible->isDead()) {
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
			player->save(zip);

			actors.remove(player);

			for (int i = 0; i < WORLD_SIZE_LATITUDE; i++) {
				for (int j = 0; j < WORLD_SIZE_LONGITUDE; j++) {
					(*maps)[i][j]->save(zip);
				}
			}

			gui->save(zip);
			zip.saveToFile("game.sav");
		}
	} catch (...) {
		cerr << "An error occurred with Engine::save" << endl;
		throw 0;
	}
}

void Engine::load() {
	try {
		bool saveGameExists;
		if (TCODSystem::fileExists("game.sav")) {
			saveGameExists = true;

		}
		engine.gui->menu.populateMenu(saveGameExists);

		Menu::MenuItemCode menuItem = engine.gui->menu.pick();

		if (menuItem == Menu::EXIT || menuItem == Menu::NONE) {
			exitGame();
		}
		else if (menuItem == Menu::NEW_GAME) {
			newGame();
			gameStatus = STARTUP;
		}
		else if (menuItem == Menu::CONTINUE) {
			continueGame();
		}
	} catch (...) {
		cerr << "An error occurred with Engine::load" << endl;
		throw 0;
	}
}

void Engine::exitGame() {
	try {
		save();
		exit(0);

	} catch (...) {
		cerr << "An error occurred with Engine::exitGame" << endl;
		throw 0;
	}
}

void Engine::newGame() {
	try {
		term();
		init();
	} catch (...) {
		cerr << "An error occurred with Engine::newGame" << endl;
		throw 0;
	}
}

void Engine::continueGame() {
	try {
		if(gameStatus == STARTUP){
			cout << "Load game from save" << endl;
			term();
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
			player->load(zip);

			maps = new vector<vector<Map*>>(WORLD_SIZE_LATITUDE);
			for (int i = 0; i < tempLatitudeSize; i++) {
				for (int j = 0; j < tempLongitudeSize; j++) {
					(*maps)[i].push_back(new Map(width, height));
					(*maps)[i][j]->load(zip);
					if (mapX == i && mapY == j) {
						currentMap = (*maps)[i][j];
						actors = (*maps)[i][j]->actors;
						actors.push(player);
					}
				}
			}

			gui->load(zip);

			gameStatus = STARTUP;
		}
		else{
			cout << "Return to game" << endl;
			engine.gui->menu.clear();

		}
	} catch (...) {
		cerr << "An error occurred with Engine::continueGame" << endl;
		throw 0;
	}
}

Engine::~Engine() {
	try {
		actors.clearAndDelete();
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

void Engine::update() {
	try {
		cout << "update 0" << endl;
		if (gameStatus == STARTUP)
			currentMap->computeFov();
		gameStatus = IDLE;
		cout << "update 1" << endl;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
		player->update();
		cout << "update 2" << endl;
		if (gameStatus == NEW_TURN) {
			cout << "update 3" << endl;
			for (Actor *actor : actors) {
				if (actor != player) {
					actor->update();
				}
			}
		}
	} catch (...) {
		cerr << "An error occurred with Engine::update" << endl;
		throw 0;
	}
}

void Engine::nextLevel(Map::TileType type) {
	try {
		int heroX = DEFAULT_PLAYER_START_X;
		int heroY = DEFAULT_PLAYER_START_Y;
		if (type == Map::TileType::TOP_EDGE) {
			heroX = player->x;
			heroY = DEFAULT_MAP_HEIGHT - 1;
			if (mapY < WORLD_SIZE_LATITUDE - 1) {
				mapY++;
			} else {
				mapY = 0;
			}
		}
		if (type == Map::TileType::RIGHT_EDGE) {
			heroX = 0;
			heroY = player->y;
			if (mapX < WORLD_SIZE_LONGITUDE - 1) {
				mapX++;
			} else {
				mapX = 0;
			}
		}
		if (type == Map::TileType::BOTTOM_EDGE) {
			heroX = player->x;
			heroY = 0;
			if (mapY > 0) {
				mapY--;
			} else {
				mapY = WORLD_SIZE_LATITUDE - 1;
			}
		}
		if (type == Map::TileType::LEFT_EDGE) {
			heroX = DEFAULT_MAP_WIDTH - 1;
			heroY = player->y;
			if (mapX > 0) {
				mapX--;
			} else {
				mapX = WORLD_SIZE_LONGITUDE - 1;
			}
		}
		actors.remove(player);
		gui->clear();
		player = ActorFactory::CreateHero(heroX, heroY);
		currentMap = (*maps)[mapX][mapY];
		actors = currentMap->actors;
		actors.push(player);
		gameStatus = STARTUP;
		update();
	} catch (...) {
		cerr << "An error occurred with Engine::nextLevel" << endl;
		throw 0;
	}
}

void Engine::sendToBack(Actor *actor) {
	try {
		actors.remove(actor);
		actors.insertBefore(actor, 0);
	} catch (...) {
		cerr << "An error occurred with Engine::sendToBack" << endl;
		throw 0;
	}
}

void Engine::render() {
	try {
		TCODConsole::root->clear();
		// draw the map
		currentMap->render();

		gui->render();

		TCODConsole::root->print(1, screenHeight - 2, "HP: %d/%d",
				(int) player->destructible->hp,
				(int) player->destructible->maxHp);

		// draw the actors
		for (Actor *actor : actors) {
			if (currentMap->isInFov(actor->x, actor->y)) {
				actor->render();
			}
		}

	} catch (...) {
		cerr << "An error occurred with Engine::render" << endl;
		throw 0;
	}
}
