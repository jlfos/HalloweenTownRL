class Attacker : public Persistent {
public :
	void setPower(float power);
	float getPower();
	Attacker(float power);
	void attack(Actor *owner, Actor *target);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
private:
	float power;
};
