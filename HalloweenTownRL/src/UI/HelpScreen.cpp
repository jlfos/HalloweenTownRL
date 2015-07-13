#include "libtcod.hpp"
#include "Gui.hpp"
#include "HelpScreen.hpp"
#include "../Tile/TileColors.hpp"
#include "Console/Message.hpp"

HelpScreen::HelpScreen(){
	//TODO remove this hard coded crap
	log.push(new Message("Up/Down/Left/Right  ........ move/attack", TileColors::white));
	log.push(new Message("'i'  ........ inventory", TileColors::white));
	log.push(new Message("'l'  ........ look mode", TileColors::white));
	log.push(new Message("'L'  ........ open Event Log", TileColors::white));
	log.push(new Message("'g   ........ pick up item", TileColors::white));
	log.push(new Message("'.'  ........ wait", TileColors::white));
	log.push(new Message("Press ESC to return", TileColors::white));
}

HelpScreen::~HelpScreen(){
	log.clearAndDelete();
}

void HelpScreen::Show(){

	//TODO Pull this out and make it generic for the other screens

	int y=1;
	TCODConsole::root->clear();

	float colorCoef=0.4f;
	for(Message *message : log) {
		TCODConsole::root->setDefaultForeground(message->getBackgroundColor() * colorCoef);
		TCODConsole::root->print(1, y, message->getText().c_str(), message->getBackgroundColor());
		y++;
		if ( colorCoef < 1.0f ) {
			colorCoef+=0.3f;
		}
	}
	TCODConsole::root->flush();
}
