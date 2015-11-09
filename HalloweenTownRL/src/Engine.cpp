#include <iostream>
#include <chrono>
#include <vector>
#include <time.h>
#include "libtcod.hpp"
#include "Actor/Actor.hpp"
#include "Destructible/Destructible.hpp"
#include "Engine.hpp"
#include "UI/Gui.hpp"
#include "LoggerWrapper.hpp"
#include "Maps/CityMapGenerator.hpp"
#include "Maps/ForestMapGenerator.hpp"
#include "Maps/MapGenerator.hpp"
#include "MonsterEntry.hpp"
#include "Maps/RoadMapGenerator.hpp"
#include "Tile/TileColors.hpp"
#include "Tile/TileCharacters.hpp"
#include "UI/VictoryScreen.hpp"
#include "UI/BorderScreen.hpp"
#include "UI/Console/ConsoleFrame.hpp"

	const int DEFAULT_MAP_HEIGHT = 43;
	const int DEFAULT_MAP_WIDTH = 80;
	const int DEFAULT_PLAYER_START_X = 10;
	const int DEFAULT_PLAYER_START_Y = 10;
	const int DEFAULT_MAP_X = 1;
	const int DEFAULT_MAP_Y = 1;

#ifndef E_LOG
//#define E_LOG
#endif

Engine::Engine(int screenWidth, int screenHeight) :
		gameStatus(STARTUP), fovRadius(15), screenWidth(screenWidth), screenHeight(screenHeight),
		currentTime(10, 00), incrementTime(false), bossMapX(0), bossMapY(0), playerMapX(0), playerMapY(0),
		WORLD_SIZE_LATITUDE(0), WORLD_SIZE_LONGITUDE(0){
	try {
		currentMap = nullptr;
		maps = nullptr;
		player = nullptr;

		TCODConsole::initRoot(screenWidth, screenHeight, "HalloweenTown",
				false);

		gui = new Gui();



	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::Engine");
		throw 0;
	}
}

void Engine::Init() {
	try {
#ifdef E_LOG
		LoggerWrapper::Debug("Initializing Engine");
#endif
		player = ActorFactory::CreateHero(DEFAULT_PLAYER_START_X,
				DEFAULT_PLAYER_START_Y);
		std::vector<std::vector<Engine::MapType>> mapTypes{
				{Engine::MapType::FOREST_NORTH, Engine::MapType::FOREST_NORTH, Engine::MapType::FOREST_NORTH, Engine::MapType::FOREST_NORTH },
				{Engine::MapType::CITY_BOSS , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::FOREST_NORTH },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::FOREST_NORTH },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::ROAD_EW },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::FOREST_SOUTH },
				{Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::CITY , Engine::MapType::FOREST_SOUTH },
				{Engine::MapType::FOREST_SOUTH, Engine::MapType::FOREST_SOUTH, Engine::MapType::FOREST_SOUTH, Engine::MapType::FOREST_SOUTH }
		};
		maps = CreateMaps(mapTypes);
		WORLD_SIZE_LATITUDE = mapTypes.size();
		WORLD_SIZE_LONGITUDE = mapTypes.front().size();
		actors = currentMap->actors;
		actors.push(player);
		gui->PushMessage(TileColors::red,
				"Welcome stranger!\nPrepare to perish in the horrors of Halloween Town.");
		gameStatus = STARTUP;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::Init");
		throw 0;
	}
}

std::vector<std::vector<Map*>> *Engine::CreateMaps(std::vector<std::vector<Engine::MapType>> mapTypes) {
	try {
#ifdef E_LOG
		LoggerWrapper::Debug("Creating Maps");
#endif
		std::vector<std::vector<Map*>> *maps = new std::vector<std::vector<Map*>>();
		for(unsigned int i = 0; i<mapTypes.size(); i++)
		{

			maps->push_back(std::vector<Map*>());
			for(unsigned int j = 0; j<mapTypes.at(i).size(); j++)
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
					playerMapX = j;
					playerMapY = i;
					generator = new RoadMapGenerator(MapGenerator::Orientation::EAST);
					break;
				case Engine::MapType::CITY:
					generator = new CityMapGenerator(false);
					break;
				case Engine::MapType::CITY_BOSS:
					generator = new CityMapGenerator(true);
					bossMapX = j;
					bossMapY = i;
					break;
				default:
					throw 0;
					break;
				}

#ifdef E_LOG
		LoggerWrapper::Debug("Constructing Map " + std::to_string(i) + " " + std::to_string(j));
#endif
				Map* temp = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, generator);
				if(firstMapFlag)
					currentMap = temp;
				(*maps).back().push_back(temp);
			}
		}

		return maps;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Engine::CreateMaps");
		throw 0;
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
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::Term");
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
			zip.putInt(playerMapX);
			zip.putInt(playerMapY);
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
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::Save");
		throw 0;
	}
}

void Engine::Load() {
	try {
		bool saveGameExists;
		if (TCODSystem::fileExists("game.sav")) {
			saveGameExists = true;

		}
		engine.gui->PopulatePauseMenu(saveGameExists);

		std::string menuItem = engine.gui->PauseMenuPick();

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
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::load");
		throw 0;
	}
}

void Engine::ExitGame() {
	try {
		Save();
		exit(0);

	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::exitGame");
		throw 0;
	}
}

void Engine::NewGame() {
	try {
		Term();
		Init();
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::newGame");
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
			playerMapX = zip.getInt();
			playerMapY = zip.getInt();
			int tempLatitudeSize = zip.getInt();
			int tempLongitudeSize = zip.getInt();
			if (tempLatitudeSize != WORLD_SIZE_LATITUDE
					|| tempLongitudeSize != WORLD_SIZE_LONGITUDE) {
				LoggerWrapper::Error("An error occurred with loading the save file.");
				return;
			}

			player = new Actor(0, 0, 0, nullptr, TileColors::white);
			player->Load(zip);

			maps = new std::vector<std::vector<Map*>>(WORLD_SIZE_LATITUDE);
			for (int i = 0; i < tempLatitudeSize; i++) {
				for (int j = 0; j < tempLongitudeSize; j++) {
//					(*maps)[i].push_back(new Map(width, height));
					(*maps)[i][j]->Load(zip);
					if (playerMapX == i && playerMapY == j) {
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
			engine.gui->PauseMenuClear();

		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::ContinueGame");
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
				if (i == playerMapX && j == playerMapY && player != nullptr) {
					(*maps)[i][j]->actors.remove(player);
				}
				delete (*maps)[i][j];
			}
		}

		if (maps != nullptr) {
			delete maps;
			maps = nullptr;
		}

	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::~Engine");
		throw 0;
	}
}

void Engine::BossUpdate() {
	try {
		int xDiffAbs = abs(playerMapX - bossMapX);
		int yDiffAbs = abs(playerMapY - bossMapY);
		if(xDiffAbs == 0 && yDiffAbs == 0)
			return;
		if (xDiffAbs >= yDiffAbs) {
			if (bossMapX < playerMapX) {
				engine.gui->PushMessage(TileColors::lightGrey,
						"Fate pulls you to the East");
			} else {
				engine.gui->PushMessage(TileColors::lightGrey,
						"Fate pulls you to the West");
			}
		} else {
			if (bossMapY < playerMapY) {
				engine.gui->PushMessage(TileColors::lightGrey,
						"Fate pulls you to the North");
			} else {
				engine.gui->PushMessage(TileColors::lightGrey,
						"Fate pulls you to the South");
			}
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Engine::BossUpdate");
		throw 0;
	}
}

void Engine::Update() {
	try {

		if (gameStatus == STARTUP){
			currentMap->computeNonplayerLights();
			currentMap->ComputeFov();

		}
		gameStatus = IDLE;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
		player->Update();
		if (gameStatus == NEW_TURN) {
			currentMap->computeNonplayerLights();
			if(incrementTime)
				currentTime.IncrementMinutes();
			incrementTime = !incrementTime;
			for (Actor *actor : actors) {
				if (actor != player) {
					actor->Update();
				}
			}
		}
		else if(gameStatus == VICTORY){
			VictoryScreen screen;
			screen.Show();
		}
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::Update");
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
			if (playerMapY > 0) {
				playerMapY--;
			} else {
				unsigned int descriptionWidth = 45;
				std::string tooMuchGravity = "You've been walking for what seems like hours. Your shoulders stoop, your arms hang heavy, and you can barely seem to pick up your legs. Every step seems to add another ten-pound weight to your back. Finally you fall to your knees, but you won't stop there. You crawl on, but soon your arms give out. You belly-crawl a few yards more, but then the gravity is so intense you can't lift your chest to breathe. You must turn back.";
				ConsoleUI description(tooMuchGravity, descriptionWidth, engine.screenWidth/5, engine.screenHeight/5 );
				description.frame = new ConsoleFrame("Attempt to leave", TileColors::white);
				description.Display();
				description.Flush();
				bool descriptionMode = true;
				while(descriptionMode){
					TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
					switch(lastKey.vk){
					case TCODK_ESCAPE:{
						descriptionMode = false;
						break;
					}
					default:
						break;
					}

				}
				description.Clear();
				description.Display();
				description.Flush();
//				gui->PushMessage(TileColors::red,
//						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::RIGHT_EDGE) {
			heroX = 0;
			heroY = player->y;
			if (playerMapX < WORLD_SIZE_LONGITUDE - 1) {
				playerMapX++;
			} else {
				BorderScreen border;
				border.Show();
//				gui->PushMessage(TileColors::red,
//						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		if (type == Map::TileType::BOTTOM_EDGE) {
			heroX = player->x;
			heroY = 0;
			if (playerMapY < WORLD_SIZE_LATITUDE -1) {
				playerMapY++;
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
			if (playerMapX > 0) {
				playerMapX--;
			} else {
				gui->PushMessage(TileColors::red,
						"An invisible force keeps you from moving forward");
				gameStatus = IDLE;
				return;
			}
		}
		actors.remove(player);
		currentMap->TimeLastSeen(new Time(currentTime.GetHour(), currentTime.GetMinutes()));
		currentMap = (*maps)[playerMapY][playerMapX];
		bool populateFlag = false;
		if(currentMap->TimeLastSeen()){
			if(currentTime.ElapsedMinutes((*currentMap->TimeLastSeen())) >= 60)
				populateFlag = true;
		}
		else{ //Assume player has never visited
			populateFlag = true;
		}

		BossUpdate();
		if(populateFlag)
			currentMap->PopulateActors();
		actors = currentMap->actors;
		player->x = heroX;
		player->y = heroY;
		actors.push(player);
		gameStatus = STARTUP;
		Update();
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::nextLevel");
		throw 0;
	}
}

void Engine::SendToBack(Actor *actor) {
	try {
		actors.remove(actor);
		actors.insertBefore(actor, 0);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred with Engine::sendToBack");
		throw 0;
	}
}

void Engine::Render() {
	try {
//		TCODSystem::forceFullscreenResolution(800, 400);
		TCODConsole::root->clear();
		// draw the map
		gui->Render();
		currentMap->Render();
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
		LoggerWrapper::Error("An error occurred with Engine::Render");
		throw 0;
	}
}

std::string Engine::GetItemDescription(std::string name){
	try {
		std::map<std::string, std::string> itemDescriptions;
		itemDescriptions["knife"] = "A blade and a handle to hold it, what more could you need?";
		itemDescriptions["nightstick"] = "A short, heavy club often used in law enforcement.";
		itemDescriptions["kris"] = "A wavy-bladed dagger originating in Southeast Asia said to contain blessings... or curses.";
		itemDescriptions["tire iron"] = "A useful tool for changing tires made of steel, also useful for bashing in skulls.";
		itemDescriptions["tree branch"] = "It may just be a chunk of wood, but it's heavy enough to make an effective bludgeon.";
		itemDescriptions["machete"] = "With a blade longer than a foot, this is great for chopping underbrush.";
		itemDescriptions["medkit"] = "A kit with medical supplies for patching medium injuries";
		itemDescriptions["baseball bat"] = "Nearly a yard long, this Louisville Slugger is weighted for maximum smashing!";
		return itemDescriptions[name];
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Engine::GetItemDescription");
		throw 0;
	}
}
