/*

 * Time.hpp
 *
 *  Created on: Mar 14, 2015
 *      Author: josh
 */
#ifndef TIME_HPP_
#define TIME_HPP_

#include <string>

using namespace std;

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
		string ConvertMinutes();
};



#endif /* TIME_HPP_ */
