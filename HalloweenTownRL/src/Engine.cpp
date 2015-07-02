#include <iostream>
#include <chrono>
#include <vector>
#include <time.h>
#include "libtcod.hpp"
#include "Actor/Actor.hpp"
#include "Destructible/Destructible.hpp"
#include "Engine.hpp"
#include "UI/Gui.hpp"
#include "Maps/CityMapGenerator.hpp"
#include "Maps/ForestMapGenerator.hpp"
#include "Maps/MapGenerator.hpp"
#include "MonsterEntry.hpp"
#include "Maps/RoadMapGenerator.hpp"
#include "Tile/TileColors.hpp"
#include "Tile/TileCharacters.hpp"

Engine::Engine(int screenWidth, int screenHeight) :
		gameStatus(STARTUP), fovRadius(10), screenWidth(screenWidth), screenHeight(screenHeight),
		currentTime(6, 00), incrementTime(false){
	try {
		currentMap = nullptr;
		maps = nullptr;
		player = nullptr;

		TCODConsole::initRoot(screenWidth, screenHeight, "HalloweenTown",
				false);

		gui = new Gui();
		MonsterEntry temp("goblin", "lizard thing", "????", TileCharacters::Default::G_LOWERCASE, TileColors::white);
//		monsterLog.insert(temp);


	} catch (...) {
		std::cerr << "An error occurred with Engine::Engine" << std::endl;
		throw 0;
	}
}

void Engine::Init() {
	try {

		player = ActorFactory::CreateHero(DEFAULT_PLAYER_START_X,
				DEFAULT_PLAYER_START_Y);
		std::vector<std::vector<Engine::MapType>> mapTypes{
				{Engine::MapType::FOREST_NORTH, Engine::MapType::FOREST_NORTH, Engine::MapType::FOREST_NORTH, Engine::MapType::FOREST_NORTH },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::FOREST_NORTH },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::ROAD_EW },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::FOREST_SOUTH },
				{Engine::MapType::FOREST_SOUTH, Engine::MapType::FOREST_SOUTH, Engine::MapType::FOREST_SOUTH, Engine::MapType::FOREST_SOUTH }
		};
		maps = CreateMaps(mapTypes);
		WORLD_SIZE_LATITUDE = mapTypes.size();
		WORLD_SIZE_LONGITUDE = mapTypes.front().size();
//		currentMap = (*maps)[mapX][mapY];
		actors = currentMap->actors;
		actors.push(player);
		gui->PushMessage(TileColors::red,
				"Welcome stranger!\nPrepare to perish in the horrors of Halloween Town.");
		gameStatus = STARTUP;
	} catch (...) {
		std::cerr << "An error occurred with Engine::Init" << std::endl;
		throw 0;
	}
}

std::vector<std::vector<Map*>> *Engine::CreateMaps(std::vector<std::vector<Engine::MapType>> mapTypes) {
	try {
		std::vector<std::vector<Map*>> *maps = new std::vector<std::vector<Map*>>();
		for(int i = 0; i<mapTypes.size(); i++)
//		(std::vector<Engine::MapType> v : mapTypes)
		{

			maps->push_back(std::vector<Map*>());
			for(int j = 0; j<mapTypes.at(i).size(); j++)
//			(Engine::MapType mt : v)
			{

				bool firstMapFlag = false;
				MapGenerator* generator = nullptr;
				switch(mapTypes.at(i).at(j)){
				case Engine::MapType::FOREST_NORTH:
					generator = new ForestMapGenerator(MapGenerator::Orientation::NORTH);
					break;
				case Engine::MapType::FOREST_SOUTH:
					generator = new ForestMapGenerator(MapGenerator::Orientation::SOUTH);
					break;
				case Engine::MapType::ROAD_EW:
					firstMapFlag = true;
					mapX = j;
					mapY = i;
					generator = new RoadMapGenerator(MapGenerator::Orientation::EAST);
					break;
				case Engine::MapType::CITY:
					generator = new CityMapGenerator();
					break;
				}


				Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, generator);
				temp->Init();
				if(firstMapFlag)
					currentMap = temp;
				(*maps).back().push_back(temp);
			}
		}

//		for (int i = 0; i < WORLD_SIZE_LATITUDE; i++) {
//			for (int j = 0; j < WORLD_SIZE_LONGITUDE; j++) {
//				 if (i  == 1) {
//					Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT,
//							new RoadMapGenerator());
//					if (temp) {
//						maps->at(i).push_back(temp);
//					}
//				} else {
//					Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT,
//							new ForestMapGenerator(MapGenerator::Orientation::SOUTH));
//					if (temp) {
//						maps->at(i).push_back(temp);
//					}
//				}
//				(*maps)[i][j]->Init();
//
//			}
//		}

		return maps;
	} catch (...) {
		std::cerr << "An error occurred in Engine::CreateMaps" << std::endl;
	}
}

void Engine::Term() {

	try {

		//actors.clearAndDelete();
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
		Time temp(6, 00);
		currentTime = temp;
	} catch (...) {
		std::cerr << "An error occurred with Engine::Term" << std::endl;
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
			zip.putInt(currentMap->GetWidth());
			zip.putInt(currentMap->GetHeight());
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
		std::cerr << "An error occurred with Engine::Save" << std::endl;
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

		std::string menuItem = engine.gui->menu.Pick();

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
		std::cerr << "An error occurred with Engine::load" << std::endl;
		throw 0;
	}
}

void Engine::ExitGame() {
	try {
		Save();
		exit(0);

	} catch (...) {
		std::cerr << "An error occurred with Engine::exitGame" << std::endl;
		throw 0;
	}
}

void Engine::NewGame() {
	try {
		Term();
		Init();
	} catch (...) {
		std::cerr << "An error occurred with Engine::newGame" << std::endl;
		throw 0;
	}
}

void Engine::ContinueGame() {
	try {
		if(gameStatus == STARTUP){
			std::cout << "Load game from save" << std::endl;
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
				std::cerr << "An error occurred with loading the save file." << std::endl;
				return;
			}

			player = new Actor(0, 0, 0, nullptr, TileColors::white);
			player->Load(zip);

			maps = new std::vector<std::vector<Map*>>(WORLD_SIZE_LATITUDE);
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
			std::cout << "Return to game" << std::endl;
			engine.gui->menu.Clear();

		}
	} catch (...) {
		std::cerr << "An error occurred with Engine::ContinueGame" << std::endl;
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
		std::cerr << "An error occurred with Engine::~Engine" << std::endl;
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
		std::cerr << "An error occurred with Engine::Update" << std::endl;
		throw 0;
	}
}


void Engine::NextLevel(Map::TileType type) {
	try {
		int heroX = DEFAULT_PLAYER_START_X;
		int heroY = DEFAULT_PLAYER_START_Y;

		if (type == Map::TileType::TOP_EDGE) {
			heroX = player->x;
			heroY = DEFAULT_MAP_HEIGHT - 2;
			if (mapY > 0) {
				mapY--;
			} else {
				gui->PushMessage(TileColors::red,
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
				gui->PushMessage(TileColors::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::BOTTOM_EDGE) {
			heroX = player->x;
			heroY = 0;
			if (mapY < WORLD_SIZE_LATITUDE -1) {
				mapY++;
			} else {
				gui->PushMessage(TileColors::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::LEFT_EDGE) {
			heroX = DEFAULT_MAP_WIDTH - 2;
			heroY = player->y;
			if (mapX > 0) {
				mapX--;
			} else {
				gui->PushMessage(TileColors::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		actors.remove(player);
		currentMap = (*maps)[mapY][mapX];
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
		player->x = heroX;
		player->y = heroY;
		actors.push(player);
		gameStatus = STARTUP;
		Update();
	} catch (...) {
		std::cerr << "An error occurred with Engine::nextLevel" << std::endl;
		throw 0;
	}
}

void Engine::SendToBack(Actor *actor) {
	try {
		actors.remove(actor);
		actors.insertBefore(actor, 0);
	} catch (...) {
		std::cerr << "An error occurred with Engine::sendToBack" << std::endl;
		throw 0;
	}
}

void Engine::Render() {
	try {
		TCODConsole::root->clear();
		// draw the map
		currentMap->Render();

		gui->Render();

		// draw the actors
		for (Actor *actor : actors) {
			if(actor){
				if (currentMap->IsInFov(actor->x, actor->y)) {
					actor->Render();
				}
			}
		}
		TCODConsole::flush();

	}
	catch (...) {
		std::cerr << "An error occurred with Engine::Render" << std::endl;
		throw 0;
	}
}
