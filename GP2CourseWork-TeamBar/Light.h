#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
using glm::vec4;
using glm::vec3;

#include "Component.h"

class Light :public Component
{
public:
	Light();
	~Light();

	vec4& getDiffuseColour();
	void setDiffuseColour(float r, float g, float b, float a);

	vec4& getSpecularColour();
	void setSpecularColour(float r, float g, float b, float a);

	vec3& getDirection();
	void setDirection(float x, float y, float z);

	vec3& getPosition();
	void setPosition(float x, float y, float z);

	float& getAttenuation();
	void setAttenuation(float a);

	float& getConstantAttenuation();
	void setConstantAttenuation(float c);

	float& getLinearAttenuation();
	void setLinearAttenuation(float l);

	float& getQuadraticAttenuation();
	void setQuadraticAttenuation(float q);
private:
	vec4 m_DiffuseColour;
	vec4 m_SpecularColour;

	vec3 m_LightDirection;
	vec3 m_LightPosition;

	float m_Attenuation;
	float m_ConstantAttenuation;
	float m_LinearAttenuation;
	float m_QuadraticAttenuation;
protected:
};

#endif