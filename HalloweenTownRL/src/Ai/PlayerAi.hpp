#ifndef HALLOWEENTOWN_PLAYERAI_HPP
#define HALLOWEENTOWN_PLAYERAI_HPP
class PlayerAi : public Ai {
public:
	PlayerAi();
	void Update(Actor *owner);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
	int GetLevel();
protected:
	bool MoveOrAttack(Actor *owner, int targetX, int targetY);
	void HandleActionKey(Actor *owner, int ascii);
	Actor *ChooseFromInventory(Actor *owner);
	void MoveLeft(Actor *owner);
	void MoveDown(Actor *owner);
	void MoveRight(Actor *owner);
	void MoveUp(Actor *owner);
	void LoadMenu();
private:
	void PlayerLook(Actor* player);
	void LevelUpPlayer(Actor* player);
	bool LevelUpOccurred();
	int currentLevelGoal;
	int currentExperience;
	int experienceLevel;
};
#endif
