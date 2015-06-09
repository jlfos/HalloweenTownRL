/*
 * ActorFactory.cpp
 *
 *  Created on: Feb 9, 2015
 *
 */

#ifndef HALLOWEENTOWN_ACTORFACTORY_HPP
#define HALLOWEENTOWN_ACTORFACTORY_HPP

class ActorFactory {
	public:
		enum class EnemyDifficulty {EASY, MEDIUM, HARD, VERY_HARD, INSANE, NIGHTMARE};
	    enum class MapType {CITY, ROAD, WOODS};
		static Actor *CreateHero(int x, int y);
		static Actor *CreatePotion(int x, int y);
		static Actor *CreateMonster(int x, int y, EnemyDifficulty difficulty, MapType mapType);

	private:
		static Actor *CreateMonsterForCity(int x, int y, EnemyDifficulty difficulty);
		static Actor *CreateImp(int x, int y);
		static Actor *CreateTroll(int x, int y);
		static Actor *CreateVampire(int x, int y, EnemyDifficulty difficulty);
};



#endif /* HALLOWEENTOWN_ACTORFACTORY_CPP */
