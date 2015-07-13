/*
 * Messages.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include <string>
#include "libtcod.hpp"
#include "Message.hpp"
#include "../../Tile/TileColors.hpp"
Message::Message(std::string text) : Message(text, TileColors::white) {

}

Message::Message(std::string text, const TCODColor &backgroundColor):Message(text, backgroundColor, TileColors::black){

}


Message::Message(std::string text, const TCODColor &backgroundColor, const TCODColor &foregroundColor):
		text(text), backgroundColor(backgroundColor), foregroundColor(foregroundColor){

}

Message::~Message(){

}

std::string Message::getText(){
	return text;
}

TCODColor Message::getBackgroundColor(){
	return backgroundColor;
}

TCODColor Message::getForegroundColor(){
	return foregroundColor;
}
