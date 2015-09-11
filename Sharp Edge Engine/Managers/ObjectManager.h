#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <list>

class IObject;

class ObjectManager
{
private:
	std::list < IObject* > Objects;
public:
	ObjectManager();
	~ObjectManager();
	void Run(double _dt);
};

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}





#endif