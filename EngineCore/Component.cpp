#include "Component.h"
#include "GameObject.h"

Component::Component() : Object("Component")
{
}


Component::Component(std::string name) : Object(name)
{
}


Component::~Component()
{
	LOG_TRACE("Component Deletion");
}
