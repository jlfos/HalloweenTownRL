class PlayerDestructible : public Destructible {
public :
	PlayerDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
};
