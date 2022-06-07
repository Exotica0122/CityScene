#pragma once

#include <freeglut.h>

//  position the light source 
GLfloat lightPosition[];

// a material that is all zeros
GLfloat zeroMaterial[];

// a red ambient material
GLfloat redAmbient[];

// a blue diffuse material
GLfloat blueDiffuse[];

// a red diffuse material
GLfloat redDiffuse[];

// a white specular material
GLfloat whiteSpecular[];

// the degrees of shinnines (size of the specular highlight, bigger number means smaller highlight)
GLfloat noShininess;
GLfloat highShininess;