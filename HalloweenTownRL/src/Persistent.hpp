#ifndef HALLOWEENTOWN_PERSISTENT_HPP
#define HALLOWEENTOWN_PERSISTENT_HPP
class Persistent {
public :
    virtual void Load(TCODZip &zip) = 0;
    virtual void Save(TCODZip &zip) = 0;
};
#endif
