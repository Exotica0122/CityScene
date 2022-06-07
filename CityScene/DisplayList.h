#pragma once

#include <freeglut.h>

// DisplayList
GLuint skyscraperDL;
GLuint BuildingDL;

// Draw Vertices
GLfloat vertices[][3];
// Colors of the vertices
GLfloat colors[][3];

void createSkyscraperDisplayList(void);
void drawSkyscraper(void);
void polygon(int a, int b, int c, int d);
void colorcube(void);