#include "Light.h"

Light::Light()
{
	m_DiffuseColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightDirection = vec3(0.0f, 0.0f, 0.0f);
	m_LightPosition = vec3(10.0f, 10.0f, 10.0f);
	m_Attenuation = 1.0f;
	m_ConstantAttenuation = 1.0f;
	m_QuadraticAttenuation = 0.0f;
	m_LinearAttenuation = 0.0f;
	m_Type = "Light";
}

Light::~Light()
{

}

vec4& Light::getSpecularColour()
{
	return m_SpecularColour;
}

void Light::setSpecularColour(float r, float g, float b, float a)
{
	m_SpecularColour = vec4(r, g, b, a);
}

vec4& Light::getDiffuseColour()
{
	return m_DiffuseColour;
}

void Light::setDiffuseColour(float r, float g, float b, float a)
{
	m_DiffuseColour = vec4(r, g, b, a);
}

vec3& Light::getDirection()
{
	return m_LightDirection;
}

void Light::setDirection(float x, float y, float z)
{
	m_LightDirection = vec3(x, y, z);
}

vec3& Light::getPosition()
{
	return m_LightPosition;
}

void Light::setPosition(float x, float y, float z)
{
	m_LightPosition = vec3(x, y, z);
}

float& Light::getAttenuation()
{
	return m_Attenuation;
}

void Light::setAttenuation(float c)
{
	m_Attenuation;
}

float& Light::getConstantAttenuation()
{
	return m_ConstantAttenuation;
}

void Light::setConstantAttenuation(float c)
{
	m_ConstantAttenuation;
}

float& Light::getLinearAttenuation()
{
	return m_LinearAttenuation;
}

void Light::setLinearAttenuation(float l)
{
	m_ConstantAttenuation;
}

float& Light::getQuadraticAttenuation()
{
	return m_QuadraticAttenuation;
}

void Light::setQuadraticAttenuation(float l)
{
	m_QuadraticAttenuation;
}