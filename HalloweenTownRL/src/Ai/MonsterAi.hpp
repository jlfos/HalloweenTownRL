class MonsterAi : public Ai {
public:
	MonsterAi();
	void update(Actor *owner);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
protected:
	void moveOrAttack(Actor *owner, int targetX, int targetY);
	int moveCount;
};
