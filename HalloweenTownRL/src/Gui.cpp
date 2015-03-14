#include <stdarg.h>
#include <iostream>
#include <fstream>
#include "main.hpp"

using namespace std;

static const int PANEL_HEIGHT=7;
static const int BAR_WIDTH=20;
static const int MSG_X=BAR_WIDTH+2;
static const int MSG_HEIGHT=PANEL_HEIGHT-1;

Gui::Gui() {
	try{
		con = new TCODConsole(engine.screenWidth,PANEL_HEIGHT);
	}
	catch(...){
		cerr << "An error occurred with Gui::Gui"  << endl;
		throw 0;
	}

}

Gui::~Gui() {
	try{
		delete con;
		log.clearAndDelete();
	}
	catch(...){
		cerr << "An error occurred with Gui::~Gui"  << endl;
		throw 0;
	}
}


void Gui::render() {
	try{
		// clear the GUI console
		con->setDefaultBackground(TCODColor::black);
		con->clear();

		// draw the health bar
		renderBar(1,1,BAR_WIDTH,"HP",engine.player->destructible->hp,
			engine.player->destructible->maxHp,
			TCODColor::lightRed,TCODColor::darkerRed);

		con->print(2,2, engine.currentTime.c_str());

		// draw the message log
		int y=1;
		float colorCoef=0.4f;
		for (Message *message : log) {
			con->setDefaultForeground(message->col * colorCoef);
			con->print(MSG_X,y,message->text);
			y++;
			if ( colorCoef < 1.0f ) {
				colorCoef+=0.3f;
			}
		}

		// mouse look
		renderMouseLook();



		// blit the GUI console on the root console
		TCODConsole::blit(con,0,0,engine.screenWidth,PANEL_HEIGHT,
			TCODConsole::root,0,engine.screenHeight-PANEL_HEIGHT);

	}
	catch(...){
		cerr << "An error occurred with Gui::render"  << endl;
		throw 0;
	}
}

void Gui::renderBar(int x, int y, int width, const char *name,
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
		con->setDefaultForeground(TCODColor::white);
		con->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER,
			"%s : %g/%g", name, value, maxValue);
	}
	catch(...){
		cerr << "An error occurred with Gui::renderBar"  << endl;
		throw 0;
	}
}

void Gui::clear(){
	try{
		log.clearAndDelete();
	}
	catch(...){
		cerr << "An error occurred with Gui::clear"  << endl;
		throw 0;
	}
}

Gui::Message::Message(const char *text, const TCODColor &col) :
	text(strdup(text)),col(col) {
}

void Gui::save(TCODZip &zip){
	try{
		zip.putInt(log.size());
		for(Message *it : log){
			zip.putString((it)->text);
			zip.putColor(&(it)->col);
		}
	}
	catch(...){
		cerr << "An error occurred with Gui::save"  << endl;
		throw 0;
	}
}

void Gui::load(TCODZip &zip){
	try{
		int nbMessages=zip.getInt();
		while(nbMessages > 0){
			const char *text=zip.getString();
			TCODColor col = zip.getColor();
			message(col, text);
			nbMessages--;
		}
	}
	catch(...){
		cerr << "An error occurred with Gui::load"  << endl;
		throw 0;
	}
}

Gui::Message::~Message() {
	try{
		free(text);
	}
	catch(...){
		cerr << "An error occurred with Gui::~Message"  << endl;
		throw 0;
	}
}

void Gui::renderMouseLook() {
	try{
		if (! engine.currentMap->isInFov(engine.mouse.cx, engine.mouse.cy)) {
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
				strcat(buf,actor->name);
			}
		}
		// display the list of actors under the mouse cursor
		con->setDefaultForeground(TCODColor::lightGrey);
		con->print(1,0,buf);
	}
	catch(...){
		cerr << "An error occurred with Gui::renderMouseLook"  << endl;
		throw 0;
	}
}

void Gui::message(const TCODColor &col, const char *text, ...) {
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
			if ( log.size() == MSG_HEIGHT ) {
				Message *toRemove=log.get(0);
				log.remove(toRemove);
				delete toRemove;
			}

			// detect end of the line
			lineEnd=strchr(lineBegin,'\n');
			if ( lineEnd ) {
				*lineEnd='\0';
			}

			// add a new message to the log
			Message *msg=new Message(lineBegin, col);
			log.push(msg);

			// go to next line
			lineBegin=lineEnd+1;
		} while ( lineEnd );
	}
	catch(...){
		cerr << "An error occurred with Gui::message"  << endl;
		throw 0;
	}
}
