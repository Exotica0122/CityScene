#include "Drone.h"
#include "Ground.h"
#include "Shape.h"

float droneYawHeading = 0.0; // degrees in facing direction
float dronePitchHeading = 0.0f; // degrees in facing direction
float droneRollHeading = 0.0f; // degrees in facing direction

GLfloat dronePosition[3] = { GROUND_WIDTH / 2, BODY_RADIUS / 2, GROUND_LENGTH / 2 };

const float droneSpeed = 8.0f;  // Metres per second

float thetaPropellar = 0.0f;

void initDroneCenterPosition(void)
{
	dronePosition[0] = GROUND_WIDTH / 2;
	dronePosition[1] = BODY_RADIUS / 2;
	dronePosition[2] = GROUND_LENGTH / 2;
}

void drawDrone(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteAmbient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, midShininess);

	//glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	// moving the drone
	glTranslatef(dronePosition[0], dronePosition[1], dronePosition[2]);
	glRotated(droneYawHeading, 0, 1, 0); // yaw rotate
	glRotated(dronePitchHeading, 1, 0, 0); // pitch rotate
	glRotated(droneRollHeading, 0, 0, 1); // roll rotate

	// draw the body
	drawBody();

	//drawLeg();

	glPushMatrix();
	glRotated(45, 0, 1, 0);

	// left side arm
	glPushMatrix();
	glTranslated(-(BODY_RADIUS * 1.5), 0, 0);
	drawArm(1);
	glPopMatrix();

	// right side arm
	glPushMatrix();
	glTranslated(BODY_RADIUS * 1.5, 0, 0);
	glRotated(180, 0, 1, 0);
	drawArm(1);
	glPopMatrix();

	// front arm
	glPushMatrix();
	glTranslated(0, 0, BODY_RADIUS * 1.5);
	glRotated(90, 0, 1, 0);
	drawArm(-1);
	glPopMatrix();

	// back arm
	glPushMatrix();
	glTranslated(0, 0, -(BODY_RADIUS * 1.5));
	glRotated(-90, 0, 1, 0);
	drawArm(-1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void drawBody(void)
{
	glPushMatrix();

	glPushMatrix();
	// squash the y axis
	glScaled(BODY_RADIUS, BODY_Y_SCALE, BODY_RADIUS);

	gluSphere(sphereQuadric, BODY_RADIUS, 50, 50);

	glTranslated(0.0, BODY_RADIUS, 0.0);
	glPopMatrix();

	//glPushMatrix();
	//glTranslated(0, 0, -BODY_RADIUS * 1.1);
	//gluSphere(sphereQuadric, 0.3, 10, 10);
	//glPopMatrix();

	glPopMatrix();
}

void drawArm(int direction)
{
	glPushMatrix();

	// bottom arm
	//glTranslated(-(DRONE_ARM_LENGTH / 2.0), 0, 0);

	glPushMatrix();
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(cylinderQuadric, DRONE_ARM_WIDTH, DRONE_ARM_WIDTH * 1.5, DRONE_ARM_LENGTH, 10, 10);
	glPopMatrix();

	// arm cap
	glPushMatrix();
	gluSphere(sphereQuadric, DRONE_ARM_WIDTH * 1.5, 10, 10);
	glPopMatrix();

	// up-right arm

	glPushMatrix();
	glRotated(90, -1, 0, 0);
	gluCylinder(cylinderQuadric, DRONE_ARM_WIDTH, DRONE_ARM_WIDTH, DRONE_ARM_LENGTH / 4, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, DRONE_ARM_LENGTH / 4, 0);
	drawPropeller();
	glPopMatrix();

	glPopMatrix();
}

void drawLeg(void)
{
	glPushMatrix();

	glTranslated(BODY_RADIUS * 0.4, -BODY_Y_SCALE * 0.75, BODY_RADIUS * 0.4);
	glRotated(90, 1, 0, 0);
	//gluCylinder(cylinderQuadric, DRONE_ARM_WIDTH, DRONE_ARM_WIDTH, DRONE_ARM_LENGTH / 4, 10, 10);
	glPushMatrix();
	gluSphere(sphereQuadric, DRONE_ARM_WIDTH, 10, 10);
	glPopMatrix();
	glPopMatrix();
}

void drawPropeller(void)
{
	glPushMatrix();

	glRotated(thetaPropellar, 0, 1, 0);

	// draw first propeller wing
	glPushMatrix();
	glTranslated(-(PROPELLER_LENGTH / 2.0), 0, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(cylinderQuadric, PROPELLER_WIDTH, PROPELLER_WIDTH, PROPELLER_LENGTH, 10, 10);
	glPopMatrix();

	// draw second propeller wing
	glPushMatrix();
	glTranslated(0, 0, -(PROPELLER_LENGTH / 2.0));
	gluCylinder(cylinderQuadric, PROPELLER_WIDTH, PROPELLER_WIDTH, PROPELLER_LENGTH, 10, 10);
	glPopMatrix();

	//draw the first propellar end caps
	glPushMatrix();
	glTranslated(-(PROPELLER_LENGTH / 2), 0.0, 0.0);
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(PROPELLER_LENGTH / 2, 0.0, 0.0);
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	// draw the second propellar end caps
	glPushMatrix();
	glTranslated(0.0, 0.0, -(PROPELLER_LENGTH / 2));
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.0, PROPELLER_LENGTH / 2);
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	//draw the propellar center
	glPushMatrix();
	gluSphere(sphereQuadric, PROPELLER_WIDTH * 1.5, 6, 6);
	glPopMatrix();

	glPopMatrix();
}