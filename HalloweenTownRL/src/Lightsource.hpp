/*
 * Lightsource.hpp
 *
 *  Created on: Jul 2, 2015
 */

#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP
#include "Persistent.hpp"

class Lightsource : public Persistent {

	public:
		Lightsource(int radius);
		void Load(TCODZip &zip);
		void Save(TCODZip &zip);
		int getRadius();
		void setRadius(int radius);
	private:
		int radius;
};



#endif /* LIGHTSOURCE_HPP */
