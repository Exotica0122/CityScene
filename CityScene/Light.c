#include "Light.h"

// Simple lighting setup
GLfloat globalAmbient[] = { 0.4f, 0.4f, 0.4f, 1 };

GLfloat lightPosition[] = { 10.0, 10.0, 10.0, 1.0 };
GLfloat ambientLight[] = { 0, 0, 0, 1 };
GLfloat diffuseLight[] = { 1, 1, 1, 1 };
GLfloat specularLight[] = { 1, 1, 1, 1 };

GLfloat spotlightPos[] = { 0, 0, 0, 1.0f }; //remember fourth parameter is w (when w = 1.0 its a directional light)
GLfloat spotlightDirection[] = { 0, -1, 0 };
GLfloat spotAngle = 20.0f;

void initLights(void)
{
	GLfloat spotlightAmbient[] = {0, 1, 0, 1};

	// Configure global ambient lighting.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Configure Light 0.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glLightfv(GL_LIGHT1, GL_POSITION, spotlightPos);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, spotlightAmbient);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightDirection);
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotAngle);
	//// enable light 1
	//glEnable(GL_LIGHT1);


	// Make GL normalize the normal vectors we supply.
	glEnable(GL_NORMALIZE);

	// Enable use of simple GL colours as materials.
	//glEnable(GL_COLOR_MATERIAL);
}