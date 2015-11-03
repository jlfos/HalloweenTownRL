/*
 * LoggerWrapper.hpp
 *
 *  Created on: Nov 2, 2015
 */

#ifndef LOGGERWRAPPER_HPP_
#define LOGGERWRAPPER_HPP_

#include <string>

class LoggerWrapper {
public:
	static void Info(std::string text);
	static void Warn(std::string text);
	static void Error(std::string text);
};



#endif /* LOGGERWRAPPER_HPP_ */
