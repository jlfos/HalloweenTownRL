/*
 * ConsoleLine.hpp
 *
 *  Created on: Jul 10, 2015
 */

#ifndef CONSOLELINE_HPP
#define CONSOLELINE_HPP
#include <vector>
#include "libtcod.hpp"
#include "Message.hpp"
#include "ConsoleRectangle.hpp"

class ConsoleLine{

public:
	ConsoleLine(Message message);
	ConsoleLine(Message message, std::vector<ConsoleRectangle> rectangles);
	Message getMessage();
	std::vector<ConsoleRectangle> getConsoleRectangle();
	static std::vector<ConsoleLine*> CreateConsoleLines(std::vector<Message> messages);
	static std::vector<ConsoleLine*> CreateConsoleLines(std::string text, unsigned int lineWidth);
private:
	Message message;
	std::vector<ConsoleRectangle> rectangles;
};



#endif /* CONSOLELINE_HPP */
