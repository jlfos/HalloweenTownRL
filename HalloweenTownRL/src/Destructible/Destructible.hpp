class Destructible : public Persistent {
public :
	Destructible(float maxHp, float defense, int exp, const char *corpseName);
	inline bool isDead() { return hp <= 0; }
	float takeDamage(Actor *owner, float damage);
	float heal(float amount);
	virtual void die(Actor *owner);
	int getExperienceReward();
	void load(TCODZip &zip);
	void save(TCODZip &zip);
	static Destructible *create(TCODZip &zip);
	void increaseTotalHealth(float health);
	void setDefense(float defense);
	float getDefense();
	float maxHp; // maximum health points
	float hp; // current health points
protected:
	enum DestructibleType{
		MONSTER, PLAYER
	};
private:
	float defense; // hit points deflected
	int experienceReward;
	const char *corpseName; // the actor's name once dead/destroyed
};
