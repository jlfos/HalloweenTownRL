class Persistent {
public :
    virtual void Load(TCODZip &zip) = 0;
    virtual void Save(TCODZip &zip) = 0;
};
