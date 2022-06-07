#include "Light.h"

void initLights(void)
{
	// Simple lighting setup
	GLfloat globalAmbient[] = { 0.f, 0.f, 0.f, 1 };

	GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat ambientLight[] = { 0, 0, 0, 1 };
	GLfloat diffuseLight[] = { 1, 1, 1, 1 };
	GLfloat specularLight[] = { 1, 1, 1, 1 };

	GLfloat spotlightPos[] = { 0.f, 0.f, 0.f, 1.0f }; //remember fourth parameter is w (when w = 1.0 its a directional light)

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

	glLightfv(GL_LIGHT1, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, spotlightPos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
	// enable light 1
	glEnable(GL_LIGHT1);

	// Make GL normalize the normal vectors we supply.
	glEnable(GL_NORMALIZE);

	// Enable use of simple GL colours as materials.
	glEnable(GL_COLOR_MATERIAL);
}