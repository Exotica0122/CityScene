#include "Lighthouse.h"

float lightTheta = 0;
int lightDirection = 1;

void drawLighthouse(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glPushMatrix();

	glRotated(270, 1, 0, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, highShininess);

	drawLighthouseBottomBody();

	glPushMatrix();
	glTranslated(0, 0, 25);

	drawLighthouseCap();
	drawLighthouseTopBody();

	glDisable(GL_TEXTURE_2D);

	drawLighthousePoles();
	drawLighthouseStand();

	glPushMatrix();
	glTranslated(0, 0, 3);
	drawStandHead();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, highShininess);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glPushMatrix();
	glTranslated(0, 0, 6);
	drawLighthouseHead();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPopMatrix();
}

void drawLighthouseBottomBody(void)
{
	glPushMatrix();
	gluCylinder(cylinderQuadric, 5.5, 4, 25, 20, 10);
	glPopMatrix();
}

void drawLighthouseCap(void)
{
	drawCircle(0, 0, 4);
}

void drawLighthouseTopBody(void)
{
	glPushMatrix();
	gluCylinder(cylinderQuadric, 4, 5, 1, 20, 5);
	glPopMatrix();
}

void drawLighthouseStand(void)
{
	glPushMatrix();
	gluCylinder(cylinderQuadric, 1, 0.5, 3, 10, 5);
	glPopMatrix();
}

void drawStandHead(void)
{
	glPushMatrix();
	gluSphere(sphereQuadric, 1, 15, 15);

	glPushMatrix();
	// animate
	glRotated(lightTheta, 0, 0, 1);
	glRotated(105, 0, 1, 0);
	glTranslated(0, 0, 0.5);
	drawLightSource();
	glPopMatrix();

	glPopMatrix();
}

void drawLightSource(void)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glPushMatrix();
	gluCylinder(cylinderQuadric, 0.2, 0.5, 1, 10, 10);

	glMaterialfv(GL_FRONT, GL_AMBIENT, lightAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightAmbient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightAmbient);
	glMaterialf(GL_FRONT, GL_SHININESS, highShininess);

	glLightfv(GL_LIGHT1, GL_POSITION, spotlightPos);

	gluCylinder(cylinderQuadric, 0.2, 1, 30, 25, 1);
	glDisable(GL_BLEND);

	//glColor4d(1, 1, 1, 1);
	glTranslated(0, 0, 1);
	drawCircle(0, 0, 0.5);

	//glLightfv(GL_LIGHT1, GL_POSITION, spotlightPos);
	glPopMatrix();
}

void drawLighthousePoles(void)
{
	glPushMatrix();

	glPushMatrix();
	glTranslated(3.5, 0, 0);
	drawLighthousePole();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3.5, 0, 0);
	drawLighthousePole();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 3.5, 0);
	drawLighthousePole();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -3.5, 0);
	drawLighthousePole();
	glPopMatrix();

	glPopMatrix();
}

void drawLighthousePole(void)
{
	glPushMatrix();

	gluCylinder(cylinderQuadric, 0.2, 0.2, 6, 5, 5);

	glPopMatrix();
}

void drawLighthouseHead(void)
{
	glPushMatrix();

	gluCylinder(cylinderQuadric, 4, 0, 2, 20, 5);

	glPopMatrix();
}

void drawCircle(float x, float y, float radius)
{
	glBegin(GL_TRIANGLE_FAN);
	//glColor3f(centerColor.x, centerColor.y, centerColor.z);
	glVertex3f(x, y, 0);
	//glColor3f(outerColor.x, outerColor.y, outerColor.z);
	for (int theta = 0; theta <= 360; theta += 10)
	{
		glVertex3f((cos(theta * DEG_TO_RAD) * radius + x), (sin(theta * DEG_TO_RAD) * radius + y), 0);
	}
	glEnd();
}

void newRandomDelay(void)
{
	lightDelay = (int) RandomFloat(100, 700);
}