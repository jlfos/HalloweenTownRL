class Destructible : public Persistent {
public :
	float maxHp; // maximum health points
	float hp; // current health points
	float defense; // hit points deflected
	const char *corpseName; // the actor's name once dead/destroyed

	Destructible(float maxHp, float defense, const char *corpseName);
	inline bool isDead() { return hp <= 0; }
	float takeDamage(Actor *owner, float damage);
	float heal(float amount);
	virtual void die(Actor *owner);

	void load(TCODZip &zip);
	void save(TCODZip &zip);
	static Destructible *create(TCODZip &zip);
protected:
	enum DestructibleType{
		MONSTER, PLAYER
	};
};
