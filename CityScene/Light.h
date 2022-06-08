#pragma once

#include <freeglut.h>

#include "Ground.h"

// Simple lighting setup
GLfloat globalAmbient[];

GLfloat lightPosition[];
GLfloat ambientLight[];
GLfloat diffuseLight[];
GLfloat specularLight[];

GLfloat spotlightPos[];
GLfloat spotlightDirection[];
GLfloat spotAngle;

void initLights(void);