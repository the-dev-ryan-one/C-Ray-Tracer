#include "Cylinder.h"
#include <math.h>

#include <iostream>
using namespace std;

float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
	float dx = dir.x;
	float dz = dir.z;
	float xc = center.x;
	float zc = center.z;
	float xo = p0.x;
	float zo = p0.z;
	float coefA = dx * dx + dz * dz;
	float coefB = 2 * (dx * (xo - xc) + dz * (zo - zc));
	float cofedC = (xo - xc)*(xo - xc) + (zo - zc)*(zo - zc) - radius*radius;
	// need to solve At^2 + Bt + C
	float descriminant = coefB * coefB - 4 * coefA * cofedC;
	if (descriminant < 0)
	{
		return -1.0;
	}

	float t1 = (-coefB - sqrt(descriminant)) / (2 * coefA);
	float t2 = (-coefB + sqrt(descriminant)) / (2 * coefA);

	//Value of tCap for which the ray will be at the same height as the cap
	float tCap = (center.y + height - p0.y) / dir.y;
	float validTCap;

	float intersectX = p0.x + tCap * dir.x;
	float intersectZ = p0.z + tCap * dir.z;
	float xDistFromCenter = abs(intersectX - center.x);
	float zDistFromCenter = abs(intersectZ - center.z);
	float distFromCenter = sqrt(xDistFromCenter * xDistFromCenter + zDistFromCenter * zDistFromCenter);

	if (distFromCenter <= radius)
	{
		return tCap;
	}
	
	if (t1 > 0)
	{
		float intersectHeight = p0.y + t1 * dir.y;

		if (!(intersectHeight > (center.y + height) || intersectHeight < center.y))
		{
			return t1;
		}
	}
	if (t2 > 0)
	{
		float intersectHeight = p0.y + t2 * dir.y;
		if (intersectHeight > (center.y + height) || intersectHeight < center.y)
		{
			return -1;
		}
		return t2;
	}
	return -1;
}


glm::vec3 Cylinder::normal(glm::vec3 p) 
{
	if (abs(p.y - (center.y + height)) < 0.01)
	{
		return glm::normalize(glm::vec3(0, 1, 0));
	}

	float xOnCylinder = p.x - center.x;
	float zOnCylinder = p.z - center.z;
	return glm::normalize(glm::vec3(xOnCylinder, 0, zOnCylinder));
}