#pragma once

#include <freeglut.h>

// Simple lighting setup
GLfloat globalAmbient[];

GLfloat lightPosition[];
GLfloat ambientLight[];
GLfloat diffuseLight[];
GLfloat specularLight[];

GLfloat spotlightPos[];

void initLights(void);