#ifndef HALLOWEENTOWN_PICKABLE_HPP
#define HALLOWEENTOWN_PICKABLE_HPP
class Pickable : public Persistent {
public :
	bool Pick(Actor *owner, Actor *wearer);
	virtual bool Use(Actor *owner, Actor *wearer);
	static Pickable *Create(TCODZip &zip);
protected:
	enum PickableType {
		HEALER
	};
};
#endif
