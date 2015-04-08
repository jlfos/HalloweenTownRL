class Attacker : public Persistent {
public :
	void SetPower(float power);
	float GetPower();
	Attacker(float power);
	void Attack(Actor *owner, Actor *target);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
private:
	float power;
};
