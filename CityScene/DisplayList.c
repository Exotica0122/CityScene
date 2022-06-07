#include "DisplayList.h"

// Draw Vertices
GLfloat vertices[][3] = { {-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0}, {1.0,1.0, 1.0}, {-1.0,1.0, 1.0} };
// Colors of the vertices
GLfloat colors[][3] = { {0.0,0.0,0.0}, {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0} };

void createSkyscraperDisplayList(void)
{
	skyscraperDL = glGenLists(1);
	glNewList(skyscraperDL, GL_COMPILE);
	drawSkyscraper();
	glEndList();
}

void drawSkyscraper(void)
{
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}

void polygon(int a, int b, int c, int d)
{
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, apartmentTexture);

	glBegin(GL_POLYGON);

	glTexCoord2f(0, 0);
	// TODO: normals for building
	//glNormal3d();
	glVertex3fv(vertices[a]);

	glTexCoord2f(0, 1);
	glVertex3fv(vertices[b]);

	glTexCoord2f(1, 1);
	glVertex3fv(vertices[c]);

	glTexCoord2f(1, 0);
	glVertex3fv(vertices[d]);

	glEnd();

	//glDisable(GL_TEXTURE_2D);
}
/*
   Map the vertices to the cube faces.
*/
void colorcube(void)
{
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}