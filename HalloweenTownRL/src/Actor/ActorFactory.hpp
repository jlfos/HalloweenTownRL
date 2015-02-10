/*
 * ActorFactory.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: josh
 */

#ifndef ACTORFACTORY_CPP_
#define ACTORFACTORY_CPP_

class ActorFactory {
	public:
		static Actor *CreateHero();
		static Actor *CreateOrc(int x, int y);
		static Actor *CreateTroll(int x, int y);
		static Actor *CreatePotion(int x, int y);
};



#endif /* ACTORFACTORY_CPP_ */
