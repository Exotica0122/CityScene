#pragma once

#include <freeglut.h>

//  position the light source 
//GLfloat lightPosition[];

// a material that is all zeros
GLfloat zeroMaterial[];

GLfloat midMaterial[];

// a red ambient material
GLfloat redAmbient[];

GLfloat whiteAmbient[];

// a blue diffuse material
GLfloat blueDiffuse[];

// a red diffuse material
GLfloat redDiffuse[];

GLfloat whiteDiffuse[];

// a white specular material
GLfloat whiteSpecular[];

// the degrees of shinnines (size of the specular highlight, bigger number means smaller highlight)
GLfloat noShininess;
GLfloat midShininess;
GLfloat highShininess;

// Lighthouse light

GLfloat lightAmbient[];
GLfloat lightDiffuse[];
GLfloat lightSpecular[];

GLfloat emissive[];

GLfloat treeLeafMaterial[];
GLfloat treeTrunkMaterial[];