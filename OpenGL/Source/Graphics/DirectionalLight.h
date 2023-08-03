#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Shader.h"

class DirectionalLight
{
public:
	DirectionalLight()
		: direction(glm::vec3(0.0f, -1.0f, 0.0f)), color(glm::vec3(1.0f)), intensity(1.0f)
	{}

	DirectionalLight(glm::vec3 direction)
		: direction(direction), color(glm::vec3(1.0f)), intensity(1.0f)
	{}

	~DirectionalLight() {}

public:
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};