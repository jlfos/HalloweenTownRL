#include "libtcod.hpp"
#include "Gui.hpp"
#include "HelpScreen.hpp"
#include "../Tile/TileColors.hpp"


HelpScreen::HelpScreen(){

}

void HelpScreen::Show(){

	int y=1;
	TCODConsole::root->clear();
	TCODList<Gui::Message *> log;
	log.clear();

	log.push(new Gui::Message("Up/Down/Left/Right  ........ move/attack", TileColors::white));
	log.push(new Gui::Message("'i'  ........ inventory", TileColors::white));
	log.push(new Gui::Message("'l'  ........ look mode", TileColors::white));
	log.push(new Gui::Message("'L'  ........ open Event Log", TileColors::white));
	log.push(new Gui::Message("'.'  ........ wait", TileColors::white));
	log.push(new Gui::Message("Press ESC to return", TileColors::white));
	float colorCoef=0.4f;
	for(Gui::Message *message : log) {
		TCODConsole::root->setDefaultForeground(message->col * colorCoef);
		TCODConsole::root->print(1, y, message->text, message->col);
//		TCODConsole::root->setChar(1, y, 32);
		y++;
		if ( colorCoef < 1.0f ) {
			colorCoef+=0.3f;
		}
	}
	TCODConsole::root->flush();
}
