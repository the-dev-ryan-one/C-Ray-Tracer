
#include <math.h>
#include <complex>
#include "Torus.h"
using namespace std;

float Torus::intersect(glm::vec3 p0, glm::vec3 dir)
{
	float tAtintersect = (center.z - p0.z) / dir.z;

	if (tAtintersect < 0)
	{
		return -1;
	}
	// add a bounding box , pc too slow
	float distToCenterSqrd = pow((p0.x + tAtintersect * dir.x - center.x), 2) 
		+ pow((p0.y + tAtintersect * dir.y - center.y), 2)
		+ pow((p0.z + tAtintersect * dir.z - center.z), 2);

	float bound = mainRadius + secondaryRadius;

	
	if (distToCenterSqrd > bound*bound*1.25)
	{
		return -1.0;
	}

	// coefficients from kityyCosine
	double a = mainRadius;
	double b = secondaryRadius;
	double dx = p0.x - center.x;
	double dy = p0.y - center.y;
	double dz = p0.z - center.z;
	double ex = dir.x;
	double ey = dir.y;
	double ez = dir.z;

	double g = 4 * a * a * (ex * ex + ey * ey);
	double h = 8 * a * a * (dx * ex + dy * ey);
	double i = 4 * a * a * (dx * dx + dy * dy);
	double j = ex * ex + ey * ey + ez * ez;
	double k = 2 * (dx * ex + dy * ey + dz * ez);
	double l = dx * dx + dy * dy + dz * dz + a * a - b * b;

	double c0 = j*j;
	double c1 = 2 * j * k;
	double c2 = 2 * j * l + k * k - g;
	double c3 = 2 * k * l - h;
	double c4 = l*l - i;



	// starting guesses
	// use powers of (0.4 + 0.9i)^1,2,3,4 from wikipeida
	std::complex<double>oldP0(0.4, 0.9);
	std::complex<double>oldQ0(-0.65, 0.72);
	std::complex<double>oldR0(-0.908, -0.297);
	std::complex<double>oldS0(-0.0959, -0.936);

	//std::complex<double>newP0(1, 1);
	//std::complex<double>newQ0(1, 1);
	//std::complex<double>newR0(1, 1);
	//std::complex<double>newS0(1, 1);

	std::complex<double>fAtP;
	std::complex<double>fAtQ;
	std::complex<double>fAtR;
	std::complex<double>fAtS;

	for (int i = 0; i < 50; i++)
	{
		fAtP = c0 * pow(oldP0, 4) + c1 * pow(oldP0, 3) + c2 * pow(oldP0, 2) + c3 * oldP0 + c4;
		oldP0 -= fAtP / ((oldP0 - oldQ0) * (oldP0 - oldR0) * (oldP0 - oldS0));

		fAtQ = c0 * pow(oldQ0, 4) + c1 * pow(oldQ0, 3) + c2 * pow(oldQ0, 2) + c3 * oldQ0 + c4;
		oldQ0 -= fAtQ / ((oldQ0 - oldP0) * (oldQ0 - oldR0) * (oldQ0 - oldS0));

		fAtR = c0 * pow(oldR0, 4) + c1 * pow(oldR0, 3) + c2 * pow(oldR0, 2) + c3 * oldR0 + c4;
		oldR0 -= fAtR / ((oldR0 - oldP0) * (oldR0 - oldQ0) * (oldR0 - oldS0));


		fAtS = c0 * pow(oldS0, 4) + c1 * pow(oldS0, 3) + c2 * pow(oldS0, 2) + c3 * oldS0 + c4;
		oldS0 -= fAtS / ((oldS0 - oldP0) * (oldS0 - oldQ0) * (oldS0 - oldR0));
	}

	double rootsList[4];
	rootsList[0] = oldP0.real();
	rootsList[1] = oldQ0.real();
	rootsList[2] = oldR0.real();
	rootsList[3] = oldS0.real();

	double imagsList[4];

	imagsList[0] = oldP0.imag();
	imagsList[1] = oldQ0.imag();
	imagsList[2] = oldR0.imag();
	imagsList[3] = oldS0.imag();

	double currMin = 100;

	for (int i = 0; i < 4; i++)
	{
		if (rootsList[i] > 0 && rootsList[i] < currMin && abs(imagsList[i]) < 0.00000001)
		{
			currMin = rootsList[i];
		}
	}

	if (currMin == 100) {
		return -1;
	}
	else {
		return currMin;
	}
		
}

glm::vec3 Torus::normal(glm::vec3 p)
{
	p = p - center;
	float alpha = mainRadius / sqrt(p.x * p.x + p.y * p.y);
	glm::vec3 n = glm::vec3(p.x, p.y, p.z) - glm::vec3(alpha * p.x, alpha * p.y, 0);
	return glm::normalize(n);

}
