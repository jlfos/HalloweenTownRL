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
#include "../LoggerWrapper.hpp"
#include "Console/Message.hpp"
#include "../Ai/PlayerAi.hpp"
#include "../Tile/TileColors.hpp"

static const int PANEL_HEIGHT=7;
static const int BAR_WIDTH=20;
static const int MSG_HEIGHT=47;

Gui::Gui() :log(new std::deque<Message>()) {
	try{
		colorCoef = 0.4f; //This came from the tutorial
		con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
		menu = nullptr;
		activeLog = new ConsoleUI(engine.screenWidth, PANEL_HEIGHT, 0, 0);

	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Gui::Gui");
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
		LoggerWrapper::Error("An error occurred with Gui::~Gui");
		throw 0;
	}
}


void Gui::Render() {
	try{
		// clear the GUI console
		con->setDefaultBackground(TileColors::black);
		con->clear();
		std::string str;

		// draw the health bar
		RenderBar(1,2,BAR_WIDTH,"HP",engine.player->destructible->hp,
			engine.player->destructible->maxHp,
			TileColors::lightRed, TileColors::darkerRed);
		str = "Traveling Magician: Majestico";
		con->print(1, 1, str.c_str());

		int startStats = 33;
		RenderBar(startStats, 1, BAR_WIDTH, "Level 1", 5,
			engine.player->destructible->maxHp,
			TCODColor::lightBlue, TCODColor::darkerBlue);

		con->print(1, 3, engine.currentTime.c_str());
		str = "STR: " + std::to_string(1);
		con->print(startStats, 2, str.c_str());
		str = "DEX: " + std::to_string(2);
		con->print(startStats, 3, str.c_str());
		str = "CON: " + std::to_string(1);
		con->print(startStats + 8, 2, str.c_str());
		str = "INT: " + std::to_string(1);
		con->print(startStats + 8, 3, str.c_str());
//		std::shared_ptr<PlayerAi> playerAi = (std::shared_ptr<PlayerAi>)engine.player->ai;

//		std::string levelText = "Level: ";
//		levelText += std::to_string(playerAi->GetLevel());
//		con->print(1, 3, levelText.c_str());

		int equipment = 55;
		RenderBar(equipment, 1, BAR_WIDTH, "Flashlight", 15,
			engine.player->destructible->maxHp,
			TCODColor::lightYellow, TCODColor::darkerYellow);
//		str = "Lightsource: flashlight";
//		con->print(equipment, 2, str.c_str());
		str = "weapon: fists";
		con->print(equipment, 2, str.c_str());
		str = "Ammo: (N/A)";
		con->print(equipment, 3, str.c_str());
		activeLog->SetConsoleLines(ConsoleLine::CreateConsoleLines(GetActiveLog()));
		activeLog->Display();

		// blit the GUI console on the root console
		TCODConsole::blit(con,0,0,engine.screenWidth,PANEL_HEIGHT,
			TCODConsole::root,0,engine.screenHeight-PANEL_HEIGHT);


		TCODConsole::root->print(1, engine.screenHeight-PANEL_HEIGHT+4, "Go west",
				engine.player->attacker->getWeapon().c_str());

//		TCODConsole::root->setChar(2, engine.screenHeight-3, TileCharacters::LEFT_ARROW);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Gui::render");
		throw 0;
	}
}

void Gui::RenderBar(int x, int y, int width, const char *name,
	float value, float maxValue, const TCODColor &barColor,
	const TCODColor &backColor) {

	try{
		std::string format = "%s : %g/%g";
		if(strcmp(name, "Level 1") == 0){
			format = "Level 1";

		}
		if(strcmp(name ,"Flashlight") == 0){
			format = "Flashlight";
		}
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
		if(format == "flashlight"){
			con->setDefaultForeground(TileColors::grey);
			con->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER,
				format.c_str(), name, value, maxValue);
			con->setDefaultForeground(TileColors::white);
		}
		else{
			con->setDefaultForeground(TileColors::white);
		con->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER,
			format.c_str(), name, value, maxValue);
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Gui::renderBar");
		throw 0;
	}
}

void Gui::Clear(){
	try{
		log->clear();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Gui::clear");
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
		LoggerWrapper::Error("An error occurred with Gui::save");
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
		LoggerWrapper::Error("An error occurred with Gui::load");
		throw 0;
	}
}

std::string Gui::PauseMenuPick() {
	try{
		if(menu)
			return menu->Pick();
		else{
			LoggerWrapper::Error("You attempted to call Pick on an uninitialized PauseMenu");
			throw 0;
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PauseMenuPick()");
		throw 0;
	}
}

void Gui::PauseMenuClear() {
	try{
		if(menu)
			return menu->Clear();
		else{
			LoggerWrapper::Error("You attempted to call Clear on an uninitialized PauseMenu");
			throw 0;
		}
	}
	catch(...){
		LoggerWrapper::Error("An error occurred in PauseMenuClear()");
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
		LoggerWrapper::Error("An error occurred with Gui::renderMouseLook");
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
		LoggerWrapper::Error("An error occurred with Gui::message");
		throw 0;
	}
}

void Gui::ShowLog(){
	try {
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
	catch (...) {
		LoggerWrapper::Error("An error occurred in Gui::ShowLog");
		throw 0;
	}
}

void Gui::PopulatePauseMenu(bool saveFileExists) {
	try {
		if(menu)
			menu->PopulateMenu(saveFileExists);
		else
			menu = new PauseMenu(saveFileExists);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Gui::PopulatePauseMenu");
		throw 0;
	}
}

void Gui::SetCharAdjusted(int x, int y, int c) {
	try {
		TCODConsole::root->setChar(x, y + 7, c);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Gui::SetCharAdjusted");
		throw 0;
	}
}

void Gui::SetForegroundAdjusted(int x, int y, TCODColor color) {
	try {
		TCODConsole::root->setCharForeground(x, y + 7, color);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Gui::SetForegroundAdjusted");
		throw 0;
	}
}

std::vector<Message> Gui::GetActiveLog() {
	try {
		std::vector<Message> activeLog;

		for(unsigned int i = 0; i < 5 && i < log->size() ; i++){

			activeLog.push_back(log->at(i));
		}
		return activeLog;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in Gui::GetActiveLog");
		throw 0;
	}
}
