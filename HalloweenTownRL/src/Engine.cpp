#include "libtcod.hpp"
#include "main.hpp"

Engine::Engine(int screenWidth, int screenHeight) : gameStatus(STARTUP),fovRadius(10),
	screenWidth(screenWidth),screenHeight(screenHeight) {

	TCODConsole::initRoot(screenWidth,screenHeight,"libtcod C++ tutorial",false);

    gui = new Gui();
}



void Engine::init(){
	player = new Actor(40,25,'@',"player",TCODColor::white);
    player->destructible=new PlayerDestructible(30,2,"your cadaver");
    player->attacker=new Attacker(5);
    player->ai = new PlayerAi();
    player->container= new Container(26);
    map = new Map(80,43);
    map->init(true);
    actors.push(player);
    gui->message(TCODColor::red,
	  "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
    gameStatus = STARTUP;
}

void Engine::term(){
	actors.clearAndDelete();
	if( map )
		delete map;
	gui->clear();
}

void Engine::save(){
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

void Engine::load(){
	engine.gui->menu.clear();
	engine.gui->menu.addItem(Menu::NEW_GAME, "New Game");

	if(TCODSystem::fileExists("game.sav")){
		engine.gui->menu.addItem(Menu::CONTINUE, "Continue");
	}

	engine.gui->menu.addItem(Menu::EXIT, "Exit");

	Menu::MenuItemCode menuItem = engine.gui->menu.pick();

	if(menuItem == Menu::EXIT || menuItem == Menu::NONE){
		exit(0);
	}
	else if(menuItem == Menu::NEW_GAME){
		engine.term();
		engine.init();
	}
	else{
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

}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
    delete gui;
}

void Engine::update() {
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

void Engine::sendToBack(Actor *actor){
	actors.remove(actor);
	actors.insertBefore(actor, 0);
}

void Engine::render() {
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
