#pragma once
#include <glm/glm.hpp>

#include <vector>

struct PointLight 
{
	PointLight()
	{
		this->Origin = { 0.0f, 0.0f, 0.0f };
	}
	PointLight(glm::vec3 origin) 
	{
		this->Origin = origin;
	}	
	PointLight(glm::vec3 origin, float intensity) 
	{
		this->Origin = origin;
		this->intensity = intensity;
	}
	~PointLight() {};

	glm::vec3 Origin;
	float intensity = 1;
};