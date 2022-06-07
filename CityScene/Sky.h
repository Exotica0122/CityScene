#pragma once

#include <freeglut.h>

#include "Texture.h"

// Sky
GLfloat skyPosition[];
float skyHeight;

// Prototypes

void initSkyPosition(void);
void drawSky(void);