#include "Building.h"

void drawTallBuilding(double r, double g, double b, double scale)
{
	glPushMatrix();

	glTranslated(0, 3 * scale, 0);
	glScaled(scale, scale, scale);

	glPushMatrix();
	glScaled(1, 3, 1);
	glColor3d(r, g, b);
	glCallList(skyscraperDL);
	glPopMatrix();

	glColor3d(0.57, 0.75, 0.77);
	drawWindows(-1, 3);

	glColor3d(0.29, 0.22, 0.11);
	glPushMatrix();
	glTranslated(0, -2.5, 1);
	drawDoor();
	glPopMatrix();

	glPopMatrix();
}

void drawBuilding(double r, double g, double b, double scale)
{
	glPushMatrix();

	glTranslated(0, 2 * scale, 0);
	glScaled(scale, scale, scale);

	glPushMatrix();
	glScaled(1, 2, 1);
	glColor3d(r, g, b);
	glCallList(skyscraperDL);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.57, 0.75, 0.77);
	drawWindows(-0.5, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.29, 0.22, 0.11);
	glTranslated(0, -1.5, 1);
	drawDoor();
	glPopMatrix();

	glPopMatrix();
}

void drawDoor(void)
{
	glPushMatrix();
	glScaled(0.4, 0.5, 0.2);
	glCallList(skyscraperDL);

	glPopMatrix();
}

void drawWindows(double startY, double endY)
{
	glPushMatrix();

	for (double i = startY; i < endY; i += 0.5)
	{
		glPushMatrix();
		glTranslated(-0.5, i, 0.9);
		drawWindow();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, i, 0.9);
		drawWindow();
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.5, i, 0.9);
		drawWindow();
		glPopMatrix();
	}

	glPopMatrix();
}

void drawWindow(void)
{
	glPushMatrix();
	glScaled(0.2, 0.2, 0.2);
	glCallList(skyscraperDL);

	glPopMatrix();
}