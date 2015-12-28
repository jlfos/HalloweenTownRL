#ifndef HALLOWEENTOWN_PLAYERAI_HPP
#define HALLOWEENTOWN_PLAYERAI_HPP

#include "libtcod.hpp"
#include "Ai.hpp"

class Actor;

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
	void MoveLeft(Actor *owner);
	void MoveDown(Actor *owner);
	void MoveRight(Actor *owner);
	void MoveUp(Actor *owner);
	void LoadMenu();
private:
	enum class InteractResult {SUCCESS, PASSIVE_ACTOR, FAILURE};

	void PlayerLook(Actor* player);
	void LevelUpPlayer(Actor* player);
	bool LevelUpOccurred();
	void viewLog();
	void ShowHelp();
	InteractResult Interact(Actor *owner, Actor* target);
	int experienceLevel;
	int currentExperience;
	int currentLevelGoal;
};
#endif
