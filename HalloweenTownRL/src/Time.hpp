/*

 * Time.hpp
 *
 *  Created on: Mar 14, 2015
 */
#ifndef HALLOWEENTOWN_TIME_HPP
#define HALLOWEENTOWN_TIME_HPP

#include <string>


class Time {
	public:
		Time();
		Time(int hour, int minutes);
		const char* c_str();
		int GetHour();
		int GetMinutes();
		int ElapsedMinutes(Time previousTime);
		void IncrementHour();
		void IncrementMinutes();
	private:
		int minutes;
		int hour;
		std::string ConvertMinutes();
};



#endif /* HALLOWEENTOWN_TIME_HPP */
