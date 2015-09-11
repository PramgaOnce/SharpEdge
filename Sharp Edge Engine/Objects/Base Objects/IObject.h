#ifndef IOBJECT_H
#define IOBJECT_H
#include <list>

#include<DirectXMath.h>
using namespace DirectX;
class IComponent;
class IObject
{


private:
	XMFLOAT4X4 WorldMatrix;
	std::list<IComponent*> Components;


public:
	IObject();
	~IObject();
	void Update(double _dt);
};

IObject::IObject()
{
}

IObject::~IObject()
{
}








#endif
