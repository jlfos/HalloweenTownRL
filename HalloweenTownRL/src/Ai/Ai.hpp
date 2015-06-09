
#ifndef HALLOWEENTOWN_AI_HPP
#define HALLOWEENTOWN_AI_HPP
class Ai : public Persistent {
public:
	virtual void Update(Actor *owner)=0;
	static Ai *Create(TCODZip &zip);
protected:
	enum AiType{
		MONSTER, PLAYER
	};
};
#endif
