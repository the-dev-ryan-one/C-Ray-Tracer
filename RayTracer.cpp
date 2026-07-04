/*==================================================================================
* COSC 363  Computer Graphics
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf   for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cone.h"
using namespace std;

const float EDIST = 40.0;
const int NUMDIV = 400;
const int MAX_STEPS = 3;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

TextureBMP texture;
TextureBMP texture2;

vector<SceneObject*> sceneObjects;



//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step) {
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(0, 25, -100);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

	glm::vec3 spotLightPos(-10, 25, -130);
	glm::vec3 spotLightDir(0, -1, 0);
	float cutOff = 10.0;

	glm::vec3 globalColor(0);
	glm::vec3 globalColor2(0);

	glm::vec3 spotlightColor(0);

	glm::vec3 lightPos2(0, 25 , -1);
	

	ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
	if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];
	//object on which the closest point of intersection is found

	//color = obj->lighting(lightPos, -ray.dir , ray.hit);						//Object's colour
	globalColor = obj->lighting(lightPos, -ray.dir, ray.hit);
	globalColor2 = obj->lighting(lightPos2, -ray.dir, ray.hit);
	spotlightColor = obj->spotLightLighting(spotLightPos, spotLightDir, cutOff, -ray.dir, ray.hit);
	
	color = 0.45f*globalColor2 + 0.45f * globalColor + 0.3f * spotlightColor;

	if (ray.index == 3) {

		int x1 = -30;
		int x2 = 30;
		int y1 = -20;
		int y2 = 30;

		//Add code for texture mapping here
		float texcoords = (ray.hit.x - x1) / (x2 - x1);
		float texcoordt = (ray.hit.y - y1) / (y2 - y1);
		if (texcoords > 0 && texcoords < 1 &&
			texcoordt > 0 && texcoordt < 1)
		{
			color = texture.getColorAt(texcoords, texcoordt);
			obj->setColor(color);
		}
	}

	if (ray.index == 9)
	{
		glm::vec3 center(-18, -8, -95);
		float r = 6;
		glm::vec3 distFromCenter = glm::normalize(ray.hit - center);
		float dz = distFromCenter.z;
		float dx = distFromCenter.x;
		float dy = distFromCenter.y;


		// uv mapping equation from wikipedia
		// use + - for rotating image , use * for zooming in and out 
		float u = -0.1 + (atan2(dz, dx) / (2 * 3.14)*3);
		float v = +0.5 + (asin(dy) / 3.14 * 1.25);

		color = texture2.getColorAt(u, v);
		obj->setColor(color);
	
	}

	//glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit


	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(sceneObjects);

	// 2nd shadow ray
	glm::vec3 lightVec2 = lightPos2 - ray.hit;
	Ray shadowRay2(ray.hit, lightVec2);
	shadowRay2.closestPt(sceneObjects);

	bool inShadow1 = (shadowRay.index > -1 && shadowRay.dist < glm::length(lightVec));
	bool inShadow2 = (shadowRay2.index > -1 && shadowRay2.dist < glm::length(lightVec2));

	if (inShadow1 && inShadow2)
	{
		color = 0.05f * obj->getColor();
	}
	else if (inShadow1) {

		// ------------
		if (sceneObjects[shadowRay.index]->isRefractive())
		{
			glm::vec3 ambient = obj->getColor();
			glm::vec3 lightPassingThrough = obj->lighting(lightPos, -ray.dir, ray.hit);
			color = 0.2f * ambient + 0.25f * lightPassingThrough;
		} else if (sceneObjects[shadowRay.index]->isTransparent())
		{
			float rho = sceneObjects[shadowRay.index]->getTransparencyCoeff() * 0.2;
			glm::vec3 ambient = obj->getColor();
			glm::vec3 lightPassingThrough = obj->lighting(lightPos, -ray.dir, ray.hit);
			color = 0.2f * ambient + rho * lightPassingThrough;
		}
		else {
			color = 0.2f * obj->getColor();
		}
		//---------------
	}
	else if (inShadow2) 
	{
		if (sceneObjects[shadowRay2.index]->isRefractive())
		{
			glm::vec3 ambient = obj->getColor();
			glm::vec3 lightPassingThrough = obj->lighting(lightPos2, -ray.dir, ray.hit);
			color = 0.2f * ambient + 0.25f * lightPassingThrough;
		}
		else if (sceneObjects[shadowRay2.index]->isTransparent())
		{
			float rho = sceneObjects[shadowRay2.index]->getTransparencyCoeff() * 0.2;
			glm::vec3 ambient = obj->getColor();
			glm::vec3 lightPassingThrough = obj->lighting(lightPos2, -ray.dir, ray.hit);
			color = 0.2f * ambient + rho * lightPassingThrough;
		}
		else {
			color = 0.2f * obj->getColor();
		}
	}

	if (obj->isTransparent() && step < MAX_STEPS)
	{
		float rho = obj->getTransparencyCoeff();
		Ray passThroughRay(ray.hit, ray.dir);
		glm::vec3 passedThroughColor = trace(passThroughRay, step + 1);
		color = passedThroughColor * rho;
	}

	if (obj->isReflective() && step < MAX_STEPS)
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	if (obj->isRefractive() && step < MAX_STEPS)
	{
		float rho = obj->getRefractionCoeff();

		glm::vec3 n = obj->normal(ray.hit);
		float eat1 = 1.0;
		float eat2 = obj->getRefractiveIndex();
		glm::vec3 g = glm::refract(ray.dir, n,(eat1 / eat2) );
		Ray refractedRay(ray.hit, g);
		refractedRay.closestPt(sceneObjects);
		glm::vec3 m = obj->normal(refractedRay.hit);
		glm::vec3 h = glm::refract(g, -m, (eat2 / eat1));

		Ray rayLeavingObject(refractedRay.hit, h);
		rayLeavingObject.closestPt(sceneObjects);
		glm::vec3 refractedcol = trace(rayLeavingObject, step + 1);
		color += (rho*refractedcol);

	}

	//----------
	// code for reflection

	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display() {
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for (int i = 0; i < NUMDIV; i++) {	//Scan every cell of the image plane
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++) {
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray
			Ray ray = Ray(eye, dir);
			glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);

			//-------------------
			// uncomment to test anti-aliasing
			//-------------------

			//glm::vec3 dir1(xp + 0.25 * cellX, yp + 0.25 * cellY, -EDIST);
			//Ray ray1 = Ray(eye, dir1);
			//glm::vec3 col1 = trace(ray1, 1);

			//glm::vec3 dir2(xp + 0.75 * cellX, yp + 0.25 * cellY, -EDIST);
			//Ray ray2 = Ray(eye, dir2);
			//glm::vec3 col2 = trace(ray2, 1);

			//glm::vec3 dir3(xp + 0.75 * cellX, yp + 0.75 * cellY, -EDIST);
			//Ray ray3 = Ray(eye, dir3);
			//glm::vec3 col3 = trace(ray3, 1);

			//glm::vec3 dir4(xp + 0.25 * cellX, yp + 0.75 * cellY, -EDIST);
			//Ray ray4 = Ray(eye, dir4);
			//glm::vec3 col4 = trace(ray4, 1);

			//float averageR = (col1.r + col2.r + col3.r + col4.r) / 4;
			//float averageG = (col1.g + col2.g + col3.g + col4.g) / 4;
			//float averageB = (col1.b + col2.b + col3.b + col4.b) / 4;

			//glColor3f(averageR, averageG, averageB);

			//glVertex2f(xp, yp);				//Draw each cell with its color value
			//glVertex2f(xp + cellX, yp);
			//glVertex2f(xp + cellX, yp + cellY);
			//glVertex2f(xp, yp + cellY);

		}
	}

	glEnd();
	glFlush();
}

//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (
// s, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize() {
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
	texture = TextureBMP("preview.bmp");
	texture2 = TextureBMP("sunTexture.bmp");

	glClearColor(0, 0, 0, 1);

	Plane* floor = new Plane(glm::vec3(-30, -20, -0), //Point A
							  glm::vec3(30, -20, -0), //Point B
							  glm::vec3(30, -20, -200), //Point C
							  glm::vec3(-30, -20, -200)); //Point D

	floor->setColor(glm::vec3(1.0, 0, 0));
	sceneObjects.push_back(floor);

	Plane* roof = new Plane(glm::vec3(-30, 30, -0), //Point A
							  glm::vec3(-30, 30, -200), //Point D
							  glm::vec3(30, 30, -200), //Point C
							  glm::vec3(30, 30, -0)); //Point B

	roof->setColor(glm::vec3(1.0, 1.0, 1.0));
	sceneObjects.push_back(roof);


	Plane*  frontWall = new Plane(glm::vec3(-30, 30, -0), //Point A
							  glm::vec3(30, 30, -0), //Point B
							  glm::vec3(30, -20, -0), //Point C
							  glm::vec3(-30, -20, -0)); //Point D

	frontWall->setColor(glm::vec3(1.0, 1, 0.0));
	sceneObjects.push_back(frontWall);

	Plane* backWall = new Plane(glm::vec3(-30, -20, -200), //Point A
						glm::vec3(30, -20, -200), //Point B
						glm::vec3(30, 30, -200), //Point C
						glm::vec3(-30, 30, -200)); //Point D

	backWall->setColor(glm::vec3(0.0, 0, 1.0));
	sceneObjects.push_back(backWall);

	Plane* leftWall = new Plane(glm::vec3(-30, -20, -0), //Point A
							  glm::vec3(-30, -20, -200), //Point B
							  glm::vec3(-30, 30, -200), //Point C
							  glm::vec3(-30, 30, -0)); //Point D

	leftWall->setColor(glm::vec3(0.0, 1, 1.0));
	sceneObjects.push_back(leftWall);

	Plane* rightWall = new Plane(glm::vec3(30, -20, -0), //Point A
							  glm::vec3(30, 30, -0), //Point B
							  glm::vec3(30, 30, -200), //Point C
							  glm::vec3(30, -20, -200)); //Point D

	rightWall->setColor(glm::vec3(1.0, 0, 1.0));
	sceneObjects.push_back(rightWall);

	Plane* mirror = new Plane
		(glm::vec3(-25, 0.5, -140),
		glm::vec3(-25, -19.5,  -140),    
		glm::vec3(-12, -19.5, -143),  
		glm::vec3(-12, 0.5, -143));

	mirror->setColor(glm::vec3(0.4, 0.4, 0.4));
	mirror->setReflectivity(true , 0.9f);
	
	sceneObjects.push_back(mirror);


	Sphere* sphere2 = new Sphere(glm::vec3(0, -15, -90), 5.0);
	sphere2->setColor(glm::vec3(0.9, 0.9, 0.9));
	sphere2->setRefractivity(true,0.8 , 1.6);
	sceneObjects.push_back(sphere2);

	Cylinder* cylinder = new Cylinder(glm::vec3(-12.5, -20, -90), 5.0, 5.0);
	cylinder->setColor(glm::vec3(1, 1, 0));
	sceneObjects.push_back(cylinder);

	Sphere* sun = new Sphere(glm::vec3(-18, -8, -95), 4);
	sceneObjects.push_back(sun);

	Sphere* transparentSphere = new Sphere(glm::vec3(15, -10, -110), 6);
	transparentSphere->setColor(glm::vec3(0.9, 0.9, 0.9));
	transparentSphere->setTransparency(true, 0.75);
	sceneObjects.push_back(transparentSphere);

	Torus* torus = new Torus(glm::vec3(0, -5, -95), 3.0, 1.5);
	torus->setColor(glm::vec3(1, 0, 1));
	sceneObjects.push_back(torus);

	Cone* cone = new Cone(glm::vec3(12.5, -20, -90), 4, 10);
	cone->setColor(glm::vec3(1, 0, 1));
	sceneObjects.push_back(cone);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Raytracing");

	glutDisplayFunc(display);
	initialize();

	glutMainLoop();
	return 0;
}
