class Ai : public Persistent {
public:
	virtual void update(Actor *owner)=0;
	static Ai *create(TCODZip &zip);
protected:
	enum AiType{
		MONSTER, PLAYER
	};
};
