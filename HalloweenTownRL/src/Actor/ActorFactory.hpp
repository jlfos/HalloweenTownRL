/*
 * ActorFactory.cpp
 *
 *  Created on: Feb 9, 2015
 *
 */

#ifndef HALLOWEENTOWN_ACTORFACTORY_HPP
#define HALLOWEENTOWN_ACTORFACTORY_HPP
#include <map>
class Actor;

class ActorFactory {
	public:
		enum class EnemyDifficulty {EASY, MEDIUM, HARD, VERY_HARD, INSANE, NIGHTMARE};
	    enum class MapType {CITY, ROAD, WOODS};
		static Actor *CreateHero(int x, int y);
		static Actor *CreateGremlin(int x, int y);
		static Actor *CreateMedkit(int x, int y);
//		static Actor *CreateGiantSpider(int x, int y);
		static Actor *CreateMonster(int x, int y, EnemyDifficulty difficulty, MapType mapType);

	private:
		static Actor *CreateMonsterForCity(int x, int y, EnemyDifficulty difficulty);
		static Actor *CreateMonsterForWoods(int x, int y, EnemyDifficulty difficulty);
		static Actor *CreateSewerMutant(int x, int y);
		static Actor *CreateVampire(int x, int y, EnemyDifficulty difficulty);
		static Actor *CreateGoblin(int x, int y);
		static Actor *CreateZombie(int x, int y);
		static Actor *CreateDirewolf(int x, int y);
		static Actor *CreateDirebear(int x, int y);
		static Actor *CreateGiantInsect(int x, int y);
		static std::map<ActorFactory::EnemyDifficulty, int> GenerateSpawnChances(EnemyDifficulty difficulty);

};



#endif /* HALLOWEENTOWN_ACTORFACTORY_CPP */
