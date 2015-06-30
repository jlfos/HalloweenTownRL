/*
 * EventLog.hpp
 *
 *  Created on: Jun 29, 2015
 */

#ifndef EVENTLOG_HPP
#define EVENTLOG_HPP

#include "Menu.hpp"

class EventLog : public Menu{
public:
	EventLog();
	void PopulateMenu(bool saveGameExists);

};


#endif /* EVENTLOG_HPP */
