class MonsterAi : public Ai {
public:
	MonsterAi();
	void Update(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
protected:
	void MoveOrAttack(Actor *owner, int targetX, int targetY);
	int moveCount;
};
