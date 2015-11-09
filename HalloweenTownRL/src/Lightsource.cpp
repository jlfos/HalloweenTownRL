/*
 * Lightsource.cpp
 *
 *  Created on: Jul 2, 2015
 */
#include <iostream>
#include "libtcod.hpp"
#include "Lightsource.hpp"
#include "LoggerWrapper.hpp"

Lightsource::Lightsource(int radius): radius(radius){

}



void Lightsource::Load(TCODZip &zip){
	try{
		radius = zip.getInt();
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Lightsource::Load");
		throw 0;
	}
}


void Lightsource::Save(TCODZip &zip){
	try{
		zip.putInt(radius);
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Lightsource::Save");
		throw 0;
	}
}


int Lightsource::GetRadius(){
	try{
		return radius;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Lightsource::GetRadius");
		throw 0;
	}
}


void Lightsource::SetRadius(int radius){
	try{
		this->radius = radius;
	}
	catch(...){
		LoggerWrapper::Error("An error occurred with Lightsource::getRadius");
		throw 0;
	}
}
