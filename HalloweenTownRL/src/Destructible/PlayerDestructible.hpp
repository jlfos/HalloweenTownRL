#ifndef HALLOWEENTOWN_PLAYERDESTRUCTIBLE_HPP
#define HALLOWEENTOWN_PLAYERDESTRUCTIBLE_HPP
class PlayerDestructible : public Destructible {
public :
	PlayerDestructible(float maxHp, float defense, const char *corpseName);
	void Die(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
#endif
