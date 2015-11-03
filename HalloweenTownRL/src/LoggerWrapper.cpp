/*
 * LoggerWrapper.cpp
 *
 *  Created on: Nov 2, 2015
 */
#include <iostream>
#include "LoggerWrapper.hpp"


void LoggerWrapper::Info(std::string text) {
}

void LoggerWrapper::Warn(std::string text) {
}

void LoggerWrapper::Error(std::string text) {
	std::cerr << text << std::endl;
}




