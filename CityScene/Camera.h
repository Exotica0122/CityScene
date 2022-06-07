#pragma once

#include <freeglut.h>
#include <math.h>

#include "Angle.h"
#include "Drone.h"

// Camera
GLfloat cameraPosition[];

float cameraAngle;
float cameraHeading;
float cameraDistanceXY;
float cameraZFar;

void initCameraPosition(void);