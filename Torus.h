
#ifndef H_TORUS
#define H_TORUS

#include <glm/glm.hpp>
#include "SceneObject.h"

class Torus : public SceneObject
{
private:
	glm::vec3 center = glm::vec3(0);
	float mainRadius = 1;
	float secondaryRadius = 1;

public:
	Torus() {};

	Torus(glm::vec3 c, float rm, float rs) : center(c), mainRadius(rm), secondaryRadius(rs) {}

	float intersect(glm::vec3 p0, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_TORUS