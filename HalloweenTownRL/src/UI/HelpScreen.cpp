#include "libtcod.hpp"
#include "Gui.hpp"
#include "HelpScreen.hpp"
#include "../LoggerWrapper.hpp"
#include "Console/Message.hpp"
#include "../Tile/TileColors.hpp"


HelpScreen::HelpScreen() : ScreenUI(){
	log.clear();
	log.push_back( Message("Up/Down/Left/Right  ........ move/attack", TileColors::white));
	log.push_back( Message("'i'  ........ inventory", TileColors::white));
	log.push_back( Message("'l'  ........ look mode", TileColors::white));
	log.push_back( Message("'L'  ........ open Event Log", TileColors::white));
	log.push_back( Message("'g   ........ pick up item", TileColors::white));
	log.push_back( Message("'o   ........ open/close door", TileColors::white));
	log.push_back( Message("'.'  ........ wait", TileColors::white));
	log.push_back( Message("Press ESC to return", TileColors::white));
}

HelpScreen::~HelpScreen(){
	log.clear();
}
