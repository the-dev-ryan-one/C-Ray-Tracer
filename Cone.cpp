#include "Cone.h"
#include "math.h"

float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float x0 = p0.x - center.x;
	float y0 = p0.y - center.y;
	float z0 = p0.z - center.z;

	float k = (radius / height) * (radius / height);

	float a = dx * dx + dz * dz - k * dy * dy;
	float b = 2 * (x0 * dx + z0 * dz - k * (y0 - height) * dy);
	float c = x0 * x0 + z0 * z0 - k * (y0 - height) * (y0 - height);

	// root eqaution will bee At^2 + Bt + C
	// check descirm
	if ((b * b - 4 * a * c) < 0) return -1;

	float rootPos = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	float rootNeg = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

	glm::vec3 pointOfintersect1 = p0 + rootPos * dir;
	glm::vec3 pointOfintersect2 = p0 + rootNeg * dir;

	if ( (pointOfintersect1.y > center.y + height) && (pointOfintersect2.y > center.y + height) )
	{
		return -1;
	}


	if ((pointOfintersect1.y < center.y) && (pointOfintersect2.y < center.y))
	{
		return -1;
	}
	// cut it off below	 

	if (rootNeg < 0 && rootPos < 0) return -1;
	return rootNeg > 0 ? rootNeg : rootPos;

}

glm::vec3 Cone::normal(glm::vec3 p)
{
	float alpha = atan2((p.x - center.x) , (p.z - center.z));
	float theta = atan(radius / height);
	glm::vec3 normalVec = glm::vec3(sin(alpha)*cos(theta),sin(theta),cos(alpha) * cos(theta));
	normalVec = glm::normalize(normalVec);
	return normalVec;
}