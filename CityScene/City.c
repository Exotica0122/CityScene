#include "City.h"

void drawCity(void)
{
	glPushMatrix();

	glPushMatrix();
	//glTranslated(30, 0, -30);
	glTranslated(GROUND_WIDTH / 2, 0.01, GROUND_LENGTH / 2);

	glPushMatrix();
	glTranslated(0, 0, 100);
	drawLighthouse();
	glPopMatrix();

	//glMaterialfv(GL_FRONT, GL_AMBIENT, midMaterial);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, midMaterial);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, zeroMaterial);
	//glMaterialf(GL_FRONT, GL_SHININESS, noShininess);

	glPushMatrix();
	glTranslated(10, 0, -10);
	glCallList(BuildingDL);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-40, 0, -10);
	glCallList(BuildingDL + 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(10, 0, -10);
	glCallList(BuildingDL + 2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(40, 0, -10);
	glCallList(BuildingDL);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-100, 0, -10);
	glCallList(BuildingDL + 1);
	glPopMatrix();

	glPopMatrix();

	drawForest();

	glPopMatrix();
}

void initForest(void)
{
	for (int i = 0; i < 1000; i++)
	{
		trees[i] = (int) RandomFloat(0, 2.9);
	}
}

void drawForest(void)
{
	int counter = 0;

	for (int i = 20; i < GROUND_WIDTH; i+=20)
	{
		for (int j = 20; j < GROUND_WIDTH / 2.3; j += 20)
		{
			glPushMatrix();
			glTranslated(i, 0, j);
			glCallList(treeDL + trees[i]);
			glPopMatrix();

			counter++;
		}
	}
}