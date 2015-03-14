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
		int getHour();
		int getMinutes();
		void incrementHour();
		void incrementMinutes();
	private:
		int minutes;
		int hour;
		string convertMinutes();
};



#endif /* TIME_HPP_ */
