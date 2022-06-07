#include "Ground.h"

/*
  A simple ground plane in the XZ plane with vertex normals specified for lighting
  the top face of the ground. The bottom face is not lit.
*/

void basicGround(void)
{
	//glColor3d(0.8, 0.9, 1);

	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zeroMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, noShininess);

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, asphaltTexture);

	glPushMatrix();
	//glTranslated(0, -BODY_RADIUS / 2, 0); // down

	for (int i = 0; i < GROUND_WIDTH; i += 10)
	{
		for (int j = 0; j < GROUND_LENGTH; j += 10)
		{
			glBegin(GL_QUADS);

			glNormal3d(0, 1, 0);
			glTexCoord2f(0, 0);
			glVertex3d(i, 0, j);

			glNormal3d(0, 1, 0);
			glTexCoord2f(0, 1);
			glVertex3d(i + GROUND_GRID, 0, j);

			glNormal3d(0, 1, 0);
			glTexCoord2f(1, 1);
			glVertex3d(i + GROUND_GRID, 0, j + GROUND_GRID);

			glNormal3d(0, 1, 0);
			glTexCoord2f(1, 0);
			glVertex3d(i, 0, j + GROUND_GRID);

			glEnd();
		}
	}

	glPopMatrix();

	//glDisable(GL_TEXTURE_2D);
}