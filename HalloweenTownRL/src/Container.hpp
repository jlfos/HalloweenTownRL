class Container : public Persistent{
public:
	int size;
	TCODList<Actor *> inventory;
	Container(int size);
	~Container();
	bool Add(Actor *actor);
	void Remove(Actor *actor);
	void Load(TCODZip &zip);
	void Save(TCODZip &zip);
};
