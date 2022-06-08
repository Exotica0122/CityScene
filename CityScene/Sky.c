#include "Sky.h"
#include "Shape.h"
#include "Camera.h"
#include "Ground.h"

GLfloat skyPosition[] = { 0, 0, 0 };
float skyHeight = 100.f;

void initSkyPosition(void)
{
	skyPosition[0] = GROUND_WIDTH / 2;
	skyPosition[1] = 0;
	skyPosition[2] = GROUND_LENGTH / 2;
}

void drawSky(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zeroMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, midShininess);

	glEnable(GL_TEXTURE_2D);

	glColor3d(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, skyTexture);

	glPushMatrix();
	glTranslated(skyPosition[0], skyPosition[1], skyPosition[2]);
	glRotated(270, 1, 0, 0);
	//gluCylinder(cylinderQuadric, (cameraZFar / 2) - 5, (cameraZFar / 2) - 5, skyHeight, 50, 50);
	gluSphere(sphereQuadric, (cameraZFar) * 0.8, 30, 30);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}