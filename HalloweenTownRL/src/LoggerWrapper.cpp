/*
 * LoggerWrapper.cpp
 *
 *  Created on: Nov 2, 2015
 */
#ifndef LOGGER
#define LOGGER
#endif

#include <iostream>
#include "LoggerWrapper.hpp"


void LoggerWrapper::Info(std::string text) {
	std::cout << "INFO: " << text << std::endl;
}

void LoggerWrapper::Warn(std::string text) {
	std::cout << "WARN: " << text << std::endl;
}

void LoggerWrapper::Debug(std::string text){
	std::cout << "DEBUG: " << text << std::endl;
}

void LoggerWrapper::Error(std::string text) {
	std::cerr << text << std::endl;
}




