#include "Camera.h"

GLfloat cameraPosition[] = { 0.f, 1.f, 15.f };

float cameraAngle = 0.0f;
float cameraHeading = 0.0f;
float cameraDistanceXY = 10.f;
float cameraZFar = 300.f;

void initCameraPosition(void)
{
	cameraPosition[0] = dronePosition[0] + ((float)sin((droneYawHeading - cameraAngle) * DEG_TO_RAD)) * cameraDistanceXY;
	cameraPosition[1] = 2.5;
	cameraPosition[2] = dronePosition[2] + ((float)cos((droneYawHeading - cameraAngle) * DEG_TO_RAD)) * cameraDistanceXY;
}