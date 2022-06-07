#pragma once

#include <freeglut.h>

// drawing propeller: sides for drawArm
enum Side {
	frontLeftSide = -2,
	frontRightSide = -1,
	rearLeftSide = 1,
	rearRightSide = 2,
};

// Drone hierachical model setup values
// Dimensions of the body
#define BODY_RADIUS 1.0
#define BODY_Y_SCALE 0.5

float droneYawHeading; // degrees in facing direction
float dronePitchHeading; // degrees in facing direction
float droneRollHeading; // degrees in facing direction

GLfloat dronePosition[3];
const float droneSpeed;

// Arm dimensions
#define DRONE_ARM_LENGTH 1.1
#define DRONE_UPRIGHT_LEGTH 1.0
#define DRONE_ARM_WIDTH 0.06

// Propellar dimensions
#define PROPELLER_LENGTH 1.1
#define PROPELLER_WIDTH 0.06

float thetaPropellar;

// Prototypes

void initDroneCenterPosition(void);
void drawBody(void);
void drawArm(int direction);
void drawLeg(void);
void drawPropeller(void);