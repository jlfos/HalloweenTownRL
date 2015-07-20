#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <memory>
#include "libtcod.hpp"
#include "../Actor/Actor.hpp"
#include "../Ai/Ai.hpp"
#include "../Attacker.hpp"
#include "Console/ConsoleLine.hpp"
#include "Console/ConsoleUI.hpp"
#include "../Destructible/Destructible.hpp"
#include "../Engine.hpp"
#include "Gui.hpp"
#include "Console/Message.hpp"
#include "../Ai/PlayerAi.hpp"
#include "../Tile/TileColors.hpp"

static const int PANEL_HEIGHT=7;
static const int BAR_WIDTH=20;
static const int MSG_X=BAR_WIDTH+2;
static const int MSG_HEIGHT=47;

Gui::Gui() :log(new std::deque<Message>()) {
	try{
		colorCoef = 0.4f; //This came from the tutorial
		con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
		menu = nullptr;
		activeLog = new ConsoleUI(engine.screenWidth, PANEL_HEIGHT, 0, 0);

	}
	catch(...){
		std::cerr << "An error occurred with Gui::Gui"  << std::endl;
		throw 0;
	}

}

Gui::~Gui() {
	try{
		if(con)
			delete con;
		if(menu)
			delete menu;
		log->clear();
	}
	catch(...){
		std::cerr << "An error occurred with Gui::~Gui"  << std::endl;
		throw 0;
	}
}


void Gui::Render() {
	try{
		// clear the GUI console
		con->setDefaultBackground(TileColors::black);
		con->clear();
		// draw the health bar
		RenderBar(1,1,BAR_WIDTH,"HP",engine.player->destructible->hp,
			engine.player->destructible->maxHp,
			TileColors::lightRed, TileColors::darkerRed);



		con->print(1, 2, engine.currentTime.c_str());

		PlayerAi *playerAi = (PlayerAi*)engine.player->ai;
		std::string levelText = "Level: ";
		levelText += std::to_string(playerAi->GetLevel());
		con->print(1, 3, levelText.c_str());

			activeLog->setConsoleLines(ConsoleLine::createConsoleLines(getActiveLog()));
			activeLog->display();

		// blit the GUI console on the root console
		TCODConsole::blit(con,0,0,engine.screenWidth,PANEL_HEIGHT,
			TCODConsole::root,0,engine.screenHeight-PANEL_HEIGHT);

		TCODConsole::root->print(1, engine.screenHeight-PANEL_HEIGHT+4, "weapon: fists",
				engine.player->attacker->getWeapon().c_str());

	}
	catch(...){
		std::cerr << "An error occurred with Gui::render"  << std::endl;
		throw 0;
	}
}

void Gui::RenderBar(int x, int y, int width, const char *name,
	float value, float maxValue, const TCODColor &barColor,
	const TCODColor &backColor) {

	try{
		// fill the background
		con->setDefaultBackground(backColor);
		con->rect(x,y,width,1,false,TCOD_BKGND_SET);

		int barWidth = (int)(value / maxValue * width);
		if ( barWidth > 0 ) {
			// draw the bar
			con->setDefaultBackground(barColor);
			con->rect(x,y,barWidth,1,false,TCOD_BKGND_SET);
		}
		// print text on top of the bar
		con->setDefaultForeground(TileColors::white);
		con->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER,
			"%s : %g/%g", name, value, maxValue);
	}
	catch(...){
		std::cerr << "An error occurred with Gui::renderBar"  << std::endl;
		throw 0;
	}
}

void Gui::Clear(){
	try{
		log->clear();
	}
	catch(...){
		std::cerr << "An error occurred with Gui::clear"  << std::endl;
		throw 0;
	}
}



void Gui::Save(TCODZip &zip){
	try{
		zip.putInt(log->size());
		for(Message it : (*log)){
			zip.putString((it).getText().c_str());
			//TODO uncomment this and fix it for save to work
//			zip.putColor(&(it)->getBackgroundColor());
		}
	}
	catch(...){
		std::cerr << "An error occurred with Gui::save"  << std::endl;
		throw 0;
	}
}

void Gui::Load(TCODZip &zip){
	try{
		int nbMessages=zip.getInt();
		while(nbMessages > 0){
			const char *text=zip.getString();
			TCODColor col = zip.getColor();
			PushMessage(col, text);
			nbMessages--;
		}
	}
	catch(...){
		std::cerr << "An error occurred with Gui::load"  << std::endl;
		throw 0;
	}
}

std::string Gui::PauseMenuPick() {
	try{
		if(menu)
			return menu->Pick();
		else{
			std::cerr << "You attempted to call Pick on an uninitialized PauseMenu" << std::endl;
			throw 0;
		}
	}
	catch(...){
		std::cerr << "An error occurred in PauseMenuPick()" << std::endl;
		throw 0;
	}
}

void Gui::PauseMenuClear() {
	try{
		if(menu)
			return menu->Clear();
		else{
			std::cerr << "You attempted to call Clear on an uninitialized PauseMenu" << std::endl;
			throw 0;
		}
	}
	catch(...){
		std::cerr << "An error occurred in PauseMenuClear()" << std::endl;
		throw 0;
	}

}

void Gui::RenderMouseLook() {
	try{
		if (! engine.currentMap->IsInFov(engine.mouse.cx, engine.mouse.cy)) {
			// if mouse is out of fov, nothing to render
			return;
		}
		char buf[128]="";
		bool first=true;
		for (Actor *actor : engine.actors) {
			// find actors under the mouse cursor
			if (actor->x == engine.mouse.cx && actor->y == engine.mouse.cy ) {
				if (! first) {
					strcat(buf,", ");
				} else {
					first=false;
				}
				strcat(buf,(actor->name).c_str());
			}
		}
		// display the list of actors under the mouse cursor
		con->setDefaultForeground(TileColors::lightGrey);
		con->print(1,0,buf);
	}
	catch(...){
		std::cerr << "An error occurred with Gui::renderMouseLook"  << std::endl;
		throw 0;
	}
}

void Gui::PushMessage(const TCODColor &col, const char *text, ...) {
	try{
		// build the text
		va_list ap;
		char buf[128];
		va_start(ap,text);
		vsprintf(buf,text,ap);
		va_end(ap);

		char *lineBegin=buf;
		char *lineEnd;
		do {
			// make room for the new message
			if ( log->size() == MSG_HEIGHT ) {
				log->pop_back();
			}

			// detect end of the line
			lineEnd=strchr(lineBegin,'\n');
			if ( lineEnd ) {
				*lineEnd='\0';
			}

			// add a new message to the log
			Message msg(lineBegin, col);
			log->push_front(msg);

			// go to next line
			lineBegin=lineEnd+1;
		} while ( lineEnd );
	}
	catch(...){
		std::cerr << "An error occurred with Gui::message"  << std::endl;
		throw 0;
	}
}

void Gui::ShowLog(){
	// draw the message log
	int y=1;
	TCODConsole::root->clear();

	for(Message message : (*log)) {
		TCODConsole::root->setDefaultForeground(message.getBackgroundColor() * colorCoef);
		TCODConsole::root->print(1, y, message.getText().c_str(), message.getBackgroundColor());
//		TCODConsole::root->setChar(1, y, 32);
		y++;
		if ( colorCoef < 1.0f ) {
			colorCoef+=0.3f;
		}
	}
	TCODConsole::root->flush();
}

void Gui::PopulatePauseMenu(bool saveFileExists) {
	if(menu)
		menu->PopulateMenu(saveFileExists);
	else
		menu = new PauseMenu(saveFileExists);
}

void Gui::setCharAdjusted(int x, int y, int c) {
	TCODConsole::root->setChar(x, y + 7, c);
}

void Gui::setForegroundAdjusted(int x, int y, TCODColor color) {
	TCODConsole::root->setCharForeground(x, y + 7, color);
}

std::vector<Message> Gui::getActiveLog() {
	std::vector<Message> activeLog;
//	int i = 0;
	for(unsigned int i = 0; i < 5 && i < log->size() ; i++){

		activeLog.push_back(log->at(i));
	}
	return activeLog;
}
