#include <iostream>
#include "libtcod.hpp"
#include "main.hpp"

using namespace std;

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP),fovRadius(10),
	screenWidth(screenWidth),screenHeight(screenHeight) {
	try{
		TCODConsole::initRoot(screenWidth,screenHeight,"HalloweenTown",false);

		gui = new Gui();
	}
	catch(...){
		cerr << "An error occurred with Engine::Engine"  << endl;
		throw 0;
	}
}



void Engine::init(){
	try{
		player = ActorFactory::CreateHero(DEFAULT_PLAYER_START_X, DEFAULT_PLAYER_START_Y);
		map = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, new EmptyMapGenerator());
		map->init();
		actors.push(player);
		gui->message(TCODColor::red,
		  "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
		gameStatus = STARTUP;
	}
	catch(...){
		cerr << "An error occurred with Engine::init"  << endl;
		throw 0;
	}
}

void Engine::term(){
	try{
		actors.clearAndDelete();
		if( map )
			delete map;
		gui->clear();
	}
	catch(...){
		cerr << "An error occurred with Engine::term"  << endl;
		throw 0;
	}
}

void Engine::save(){
	try{
		if(player->destructible->isDead()){
			TCODSystem::deleteFile("game.sav");
		}
		else{
			TCODZip zip;
			//save the map first
			zip.putInt(map->width);
			zip.putInt(map->height);
			map->save(zip);

			player->save(zip);

			int size = actors.size()-1;
			zip.putInt(size);
			for(Actor *it : actors){
				if(it != player){
					(it)->save(zip);
				}
			}
			gui->save(zip);
			zip.saveToFile("game.sav");
		}
	}
	catch(...){
		cerr << "An error occurred with Engine::save"  << endl;
		throw 0;
	}
}

void Engine::load(){
	try{
		bool saveGameExists;
		if(TCODSystem::fileExists("game.sav")){
			saveGameExists = true;;
		}
		engine.gui->menu.populateMenu(saveGameExists);

		Menu::MenuItemCode menuItem = engine.gui->menu.pick();

		if(menuItem == Menu::EXIT || menuItem == Menu::NONE){
			exitGame();
		}
		else if(menuItem == Menu::NEW_GAME){
			newGame();
		}
		else if(menuItem == Menu::CONTINUE){
			continueGame();
		}
	}
	catch(...){
		cerr << "An error occurred with Engine::load"  << endl;
		throw 0;
	}
}

void Engine::exitGame(){
	try{
		exit(0);
	}
	catch(...){
		cerr << "An error occurred with Engine::exitGame"  << endl;
		throw 0;
	}
}

void Engine::newGame(){
	try{
		engine.term();
		engine.init();
	}
	catch(...){
		cerr << "An error occurred with Engine::newGame"  << endl;
		throw 0;
	}
}

void Engine::continueGame(){
	try{
		term();
		TCODZip zip;
		zip.loadFromFile("game.sav");

		int width = zip.getInt();
		int height = zip.getInt();

		map = new Map(width, height);
		map->load(zip);

		player = new Actor(0,0,0,nullptr, TCODColor::white);
		player->load(zip);
		actors.push(player);
		int nbActors = zip.getInt();
		while(nbActors > 0){
			Actor *actor = new Actor(0,0,0,nullptr, TCODColor::white);
			actor->load(zip);
			actors.push(actor);
			nbActors--;
		}

		gui->load(zip);

		gameStatus = STARTUP;
	}
	catch(...){
		cerr << "An error occurred with Engine::continueGame"  << endl;
		throw 0;
	}
}

Engine::~Engine() {
	try{
		actors.clearAndDelete();
		delete map;
		delete gui;
	}
	catch(...){
		cerr << "An error occurred with Engine::~Engine"  << endl;
		throw 0;
	}
}

void Engine::update() {
	try{
		if(gameStatus == STARTUP) map->computeFov();
		gameStatus=IDLE;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&lastKey,NULL);
		player->update();
		if(gameStatus == NEW_TURN){
			for(Actor *actor : actors){
				if(actor != player){
					actor->update();
				}
			}
		}
	}
	catch(...){
		cerr << "An error occurred with Engine::update"  << endl;
		throw 0;
	}
}


void Engine::nextLevel(Map::TileType type){
	try{
		int heroX = DEFAULT_PLAYER_START_X;
		int heroY = DEFAULT_PLAYER_START_Y;
		if(type == Map::TileType::TOP_EDGE){
			heroX = player->x;
			heroY = DEFAULT_MAP_HEIGHT-1;
		}
		if(type == Map::TileType::RIGHT_EDGE){
			heroX = 0;
			heroY = player->y;
		}
		if(type == Map::TileType::BOTTOM_EDGE){
			heroX = player->x;
			heroY = 0;
		}
		if(type == Map::TileType::LEFT_EDGE){
			heroX = DEFAULT_MAP_WIDTH-1;
			heroY = player->y;
		}
		term();
		player = ActorFactory::CreateHero(heroX, heroY);
		map = new Map(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, new EmptyMapGenerator());
		map->init();
		actors.push(player);
		gameStatus = STARTUP;
		update();
	}
	catch(...){
		cerr << "An error occurred with Engine::nextLevel" << endl;
		throw 0;
	}
}

void Engine::sendToBack(Actor *actor){
	try{
		actors.remove(actor);
		actors.insertBefore(actor, 0);
	}
	catch(...){
		cerr << "An error occurred with Engine::sendToBack"  << endl;
		throw 0;
	}
}

void Engine::render() {
	try{
		TCODConsole::root->clear();
		// draw the map
		map->render();
		gui->render();
		TCODConsole::root->print(1, screenHeight-2, "HP: %d/%d",
				(int)player->destructible->hp, (int)player->destructible->maxHp);

		// draw the actors
		for (Actor *actor : actors) {
			if( map ->isInFov(actor->x, actor->y)){
				actor->render();
			}
		}
	}
	catch(...){
		cerr << "An error occurred with Engine::render"  << endl;
		throw 0;
	}
}
