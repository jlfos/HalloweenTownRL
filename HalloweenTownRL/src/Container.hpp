#ifndef HALLOWEENTOWN_CONTAINER_HPP
#define HALLOWEENTOWN_CONTAINER_HPP
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
#endif
