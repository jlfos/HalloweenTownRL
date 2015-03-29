class PlayerAi : public Ai {
public:
	PlayerAi();
	void update(Actor *owner);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
	int getLevel();
protected:
	bool moveOrAttack(Actor *owner, int targetX, int targetY);
	void handleActionKey(Actor *owner, int ascii);
	Actor *choseFromInventory(Actor *owner);
	void MoveLeft(Actor *owner);
	void MoveDown(Actor *owner);
	void MoveRight(Actor *owner);
	void MoveUp(Actor *owner);
	void LoadMenu();
private:
	void levelUpPlayer(Actor* player);
	bool levelUpOccurred();
	int currentLevelGoal;
	int currentExperience;
	int experienceLevel;
};
