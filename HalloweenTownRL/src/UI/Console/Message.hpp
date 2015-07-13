/*
 * Message.hpp
 *
 *  Created on: Jul 10, 2015
 */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include "libtcod.hpp"
class Message {
public:
	Message(std::string text);
	Message(std::string text, const TCODColor &backGroundColor);
	Message(std::string text, const TCODColor &foreGroundColor, const TCODColor &backGroundColor);
	~Message();
	std::string getText();
	TCODColor getBackgroundColor();
	TCODColor getForegroundColor();

private:
	std::string text;
	TCODColor backgroundColor;
	TCODColor foregroundColor;

};



#endif /* MESSAGE_HPP */
