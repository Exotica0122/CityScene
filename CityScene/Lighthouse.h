#pragma once

#include <freeglut.h>

#include "Angle.h"
#include "Light.h"
#include "Material.h"

#include "Lighthouse.h"
#include "Ground.h"
#include "Shape.h"
#include "Random.h"

float lightTheta;
int lightDirection;
int lightDelay;

void drawLighthouse(void);
void drawLighthouseBottomBody(void);
void drawLighthouseTopBody(void);
void drawLighthouseCap(void);
void drawLighthouseStand(void);
void drawStandHead(void);
void drawLightSource(void);
void drawLighthousePoles(void);
void drawLighthousePole(void);
void drawLighthouseHead(void);
void drawCircle(float x, float y, float radius);
void newRandomDelay(void);