#include "stdafx.h"
#include "ObjectManager.h"
#include "../Objects/Base Objects/IObject.h"

void ObjectManager::Run(double _dt)
{
	for each (IObject* obj in Objects)
	{
		obj->Update(_dt);
	}
}