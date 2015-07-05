/*
 * Lightsource.cpp
 *
 *  Created on: Jul 2, 2015
 */
#include <iostream>
#include "libtcod.hpp"
#include "Lightsource.hpp"

Lightsource::Lightsource(int radius): radius(radius){

}



void Lightsource::Load(TCODZip &zip){
	try{
		radius = zip.getInt();
	}
	catch(...){
		std::cerr << "An error occurred with Lightsource::Load"  << std::endl;
		throw 0;
	}
}


void Lightsource::Save(TCODZip &zip){
	try{
		zip.putInt(radius);
	}
	catch(...){
		std::cerr << "An error occurred with Lightsource::Save"  << std::endl;
		throw 0;
	}
}


int Lightsource::getRadius(){
	try{
		return radius;
	}
	catch(...){
		std::cerr << "An error occurred with Lightsource::getRadius"  << std::endl;
		throw 0;
	}
}


void Lightsource::setRadius(int radius){
	try{
		this->radius = radius;
	}
	catch(...){
		std::cerr << "An error occurred with Lightsource::getRadius"  << std::endl;
		throw 0;
	}
}
