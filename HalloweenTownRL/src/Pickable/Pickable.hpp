class Pickable : public Persistent {
public :
	bool pick(Actor *owner, Actor *wearer);
	virtual bool use(Actor *owner, Actor *wearer);
	static Pickable *create(TCODZip &zip);
protected:
	enum PickableType {
		HEALER
	};
};

