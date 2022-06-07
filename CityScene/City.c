#include "City.h"

void drawCity(void)
{
	glPushMatrix();

	glPushMatrix();
	//glTranslated(30, 0, -30);
	glTranslated(GROUND_WIDTH / 2, 0, GROUND_LENGTH / 2);
	drawLighthouse();
	glPopMatrix();

	glPopMatrix();
}