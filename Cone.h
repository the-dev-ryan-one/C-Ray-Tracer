#pragma once
#ifndef H_CONE
#define H_CONE
#include <glm/glm.hpp>
#include "SceneObject.h"

class Cone : public SceneObject {
private:
	glm::vec3 center = glm::vec3(0);
	float radius = 1;
	float height = 1;

public:
	Cone() {};  //Default constructor creates a unit sphere

	Cone(glm::vec3 center, float radius, float height) : center(center), radius(radius), height(height) {}

	float intersect(glm::vec3 p0, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);
};

#endif //!H_CONE