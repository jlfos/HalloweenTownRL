/*
 * Messages.cpp

 *
 *  Created on: Jul 10, 2015
 */
#include <iostream>
#include <string>
#include <vector>
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


std::vector<Message> Message::wordWrapText(std::string text, unsigned int lineSize){
	try{
		std::vector<Message> messages;
		std::string buffer = "";
		std::string currentWord = "";
		for(char& c : text){

			if(c != ' '){ //Character is not whitespace, part of current word
				if(buffer.length() + currentWord.length()+1 < lineSize){ //Can safely add another character to this line
					currentWord.append(1, c);
				}
				else{ //Line has run out of room
					Message temp(buffer);
					messages.push_back(temp);
					buffer = "";
					currentWord.append(1, c);
				}
			}
			else{ //Whitespace character indicates end of current word
				if(buffer.length() + currentWord.length()+1 < lineSize){ //adding whitespace
					buffer.append(1, c);
					buffer += currentWord;
					currentWord = "";
				}
				else{ //Whitespace is at the end of a line, just ignore.
					Message temp(buffer);
					messages.push_back(temp);
					buffer = "";
				}
			}
		}
		if(currentWord.length()>0 && buffer.length() + currentWord.length()+2 < lineSize){
			buffer += " " + currentWord;
			currentWord = "";
			Message temp(buffer);
			messages.push_back(temp);
		}
		else{
			Message tempBuffer(buffer);
			Message temp(" " + currentWord);
			messages.push_back(tempBuffer);
			messages.push_back(temp);
		}


		return messages;
	}
	catch(...){
		std::cerr << "An error occurred in ConsoleUI::wordWrapText" << std::endl;
		throw 0;
	}

}
