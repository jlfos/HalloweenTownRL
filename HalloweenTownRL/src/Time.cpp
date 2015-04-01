#include <iostream>
#include "main.hpp"

using namespace std;

Time::Time(){
	hour = 0;
	minutes = 0;
}

Time::Time(int hour, int minutes){
	try{
		this->hour = hour;
		this->minutes = minutes;
	}
	catch(...){
		cerr << "An error occurred in Time::Time()" << endl;
	}
}

int Time::elapsedMinutes(Time previousTime){
	try{

		int tempCurrentHour = hour;
		int tempPreviousHour = previousTime.getHour();

		int elapsedTime = 0;
		if(tempCurrentHour<6)
			tempCurrentHour += 12;

		if(tempPreviousHour<6)
			tempPreviousHour += 12;

		if(tempPreviousHour>tempCurrentHour){
			cerr << "A previous time cannot be later then a current time" << endl;
			throw 0;
		}

		elapsedTime = (tempCurrentHour - tempPreviousHour)*60;
		elapsedTime += (minutes - previousTime.getMinutes());
		return elapsedTime;
	}
	catch(...){
		cerr << "An error occurred in Time::elapsedMinutes()" << endl;
	}
}
int Time::getHour(){
	try{
		return hour;

	}
	catch(...){
		cerr << "An error occurred in Time::getHour()" << endl;
	}
}
int Time::getMinutes(){
	try{
		return minutes;
	}
	catch(...){
		cerr << "An error occurred in Time::getMinutes()" << endl;
	}
}

void Time::incrementHour(){
	if(hour<12){
		hour++;
	}
	else{
		hour=1;
	}
}

void Time::incrementMinutes(){
	try{
		if(minutes<59){
			minutes++;
		}
		else{
			minutes=0;
			incrementHour();
		}
	}
	catch(...){
		cerr << "An error occurred in Time::incrementMinutes()" << endl;
	}
}

const char* Time::c_str(){
	try{
		string hourStr = to_string(hour);
		string minutesStr = convertMinutes();
		string periodStr;

		if(hour>=6 &&hour<=11)
			periodStr = "PM";
		else
			periodStr = "AM";
		string total = hourStr+":"+minutesStr+periodStr;

		return total.c_str();
	}
	catch(...){
		cerr << "An error occurred in Time::c_str()" << endl;
	}
}


string Time::convertMinutes(){
	try{
		string result;
		if(minutes < 10){
			result = "0" + to_string(minutes);
		}
		else{
			result = to_string(minutes);
		}
		return result;
	}
	catch(...){
		cerr << "An error occurred in Time::convertMinutes" << endl;
	}
}
