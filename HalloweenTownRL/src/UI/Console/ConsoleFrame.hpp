/*
 * ConsoleFrame.hpp
 *
 *  Created on: Jul 18, 2015
 */

#ifndef CONSOLEFRAME_HPP
#define CONSOLEFRAME_HPP
#include <string>
#include "libtcod.hpp"

class ConsoleFrame {
public:
	ConsoleFrame(std::string frameTitle, TCODColor frameColor);
	TCODColor getFrameColor();
	std::string getFrameTitle();

private:
	std::string frameTitle;
	TCODColor frameColor;
};

#endif /* CONSOLEFRAME_HPP */
