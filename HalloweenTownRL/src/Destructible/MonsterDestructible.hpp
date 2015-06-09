#ifndef HALLOWEENTOWN_MONSTERDESTRUCTIBLE_HPP
#define HALLOWEENTOWN_MONSTERDESTRUCTIBLE
class MonsterDestructible : public Destructible {
public :
	MonsterDestructible(float maxHp, float defense, int exp, const char *corpseName);
	void Die(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
