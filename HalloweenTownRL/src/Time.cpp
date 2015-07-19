#include <iostream>
#include "Time.hpp"


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
		std::cerr << "An error occurred in Time::Time()" << std::endl;
	}
}

int Time::ElapsedMinutes(Time previousTime){
	try{

		int tempCurrentHour = hour;
		int tempPreviousHour = previousTime.GetHour();

		int elapsedTime = 0;
		if(tempCurrentHour<6)
			tempCurrentHour += 12;

		if(tempPreviousHour<6)
			tempPreviousHour += 12;

		if(tempPreviousHour>tempCurrentHour){
			std::cerr << "A previous time cannot be later then a current time" << std::endl;
			throw 0;
		}


		elapsedTime = (tempCurrentHour*60 + minutes) - (tempPreviousHour*60 + previousTime.GetMinutes());
		return elapsedTime;
	}
	catch(...){
		std::cerr << "An error occurred in Time::elapsedMinutes()" << std::endl;
		throw 0;
	}
}
int Time::GetHour(){
	try{
		return hour;

	}
	catch(...){
		std::cerr << "An error occurred in Time::getHour()" << std::endl;
		throw 0;
	}
}
int Time::GetMinutes(){
	try{
		return minutes;
	}
	catch(...){
		std::cerr << "An error occurred in Time::getMinutes()" << std::endl;
		throw 0;
	}
}

void Time::IncrementHour(){
	if(hour<12){
		hour++;
	}
	else{
		hour=1;
	}
}

void Time::IncrementMinutes(){
	try{
		if(minutes<59){
			minutes++;
		}
		else{
			minutes=0;
			IncrementHour();
		}
	}
	catch(...){
		std::cerr << "An error occurred in Time::incrementMinutes()" << std::endl;
		throw 0;
	}
}

const char* Time::c_str(){
	try{
		std::string hourStr = std::to_string(hour);
		std::string minutesStr = ConvertMinutes();
		std::string periodStr;

		if(hour>=6 &&hour<=11)
			periodStr = "PM";
		else
			periodStr = "AM";
		std::string total = hourStr+":"+minutesStr+periodStr;

		return total.c_str();
	}
	catch(...){
		std::cerr << "An error occurred in Time::c_str()" << std::endl;
		throw 0;
	}
}


std::string Time::ConvertMinutes(){
	try{
		std::string result;
		if(minutes < 10){
			result = "0" + std::to_string(minutes);
		}
		else{
			result = std::to_string(minutes);
		}
		return result;
	}
	catch(...){
		std::cerr << "An error occurred in Time::convertMinutes" << std::endl;
		throw 0;
	}
}
