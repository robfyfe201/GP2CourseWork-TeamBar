#include "Component.h"
#include "GameObject.h"

Component::Component()
{
	m_Type = "Component";
	m_Name = "Component";
	m_Parent = NULL;
	m_Active = true;
}

Component::~Component()
{

}

void Component::init()
{

}

void Component::update()
{

}

void Component::render()
{

}

void Component::destroy()
{

}

const std::string& Component::getType()
{
	return m_Type;
}

bool Component::isActive()
{
	return m_Active;
}

void Component::toggleActive()
{
	m_Active = !m_Active;
}

void Component::setParent(GameObject *object)
{
	m_Parent = object;
}

const std::string& Component::getName()
{
	return m_Name;
}

void Component::setName(const std::string& name)
{
	m_Name = name;
}

GameObject * Component::getParent()
{
	return m_Parent;
}