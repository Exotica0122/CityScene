#include "Shape.h"

void initShape(void)
{
	//create the quadric for drawing the sphere
	sphereQuadric = gluNewQuadric();

	gluQuadricTexture(sphereQuadric, GLU_TRUE);
	gluQuadricNormals(sphereQuadric, GLU_TRUE);

	//create the quadric for drawing the cylinder
	cylinderQuadric = gluNewQuadric();

	gluQuadricTexture(cylinderQuadric, GLU_TRUE);
	gluQuadricNormals(cylinderQuadric, GLU_TRUE);
}