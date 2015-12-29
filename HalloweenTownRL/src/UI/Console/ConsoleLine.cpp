/*
 * ConsoleLine.cpp
 *
 *  Created on: Jul 10, 2015
 */
#include <vector>
#include "ConsoleLine.hpp"
#include "../../LoggerWrapper.hpp"
#include "Message.hpp"
#include "ConsoleRectangle.hpp"

ConsoleLine::ConsoleLine(Message message, std::vector<ConsoleRectangle> rectangles): message(message), rectangles(rectangles){

}

ConsoleLine::ConsoleLine(Message message): message(message) {
	this->rectangles = std::vector<ConsoleRectangle>();
}

Message ConsoleLine::getMessage(){
	return message;
}

std::vector<ConsoleRectangle> ConsoleLine::getConsoleRectangle(){
	return rectangles;
}

/**
 * Takes a vector of messages and returns a vector of consoleline pointers
 */
std::vector<ConsoleLine*> ConsoleLine::CreateConsoleLines(std::vector<Message> messages){
	try {
		std::vector<ConsoleLine*> consoleLines;
		for(Message m : messages){
			ConsoleLine* cl = new ConsoleLine(m);
			consoleLines.push_back(cl);
		}
		return consoleLines;
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ConsoleLine::CreateConsoleLines(vector)");
		throw 0;
	}

}

/**
 * Takes a string of text, word wraps it (via Message::wordWrapText) and returns a vector
 * of consoleline pointers.
 */
std::vector<ConsoleLine*> ConsoleLine::CreateConsoleLines(std::string text, unsigned int lineSize) {
	try {
		std::vector<Message> messages = Message::WordWrapText(text, lineSize);
		return CreateConsoleLines(messages);
	}
	catch (...) {
		LoggerWrapper::Error("An error occurred in ConsoleLine::CreateConsoleLines(text, int)");
		throw 0;
	}
}
