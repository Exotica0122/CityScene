#include "Material.h"

//  position the light source 
GLfloat lightPosition[] = { 10.0, 10.0, 10.0, 1.0 };

// a material that is all zeros
GLfloat zeroMaterial[] = { 0.0, 0.0, 0.0, 1.0 };

// a red ambient material
GLfloat redAmbient[] = { 0.5, 0.0, 0.0, 1.0 };

// a blue diffuse material
GLfloat blueDiffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };

// a red diffuse material
GLfloat redDiffuse[] = { 1.0, 0.0, 0.0, 1.0 };

// a white specular material
GLfloat whiteSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

// the degrees of shinnines (size of the specular highlight, bigger number means smaller highlight)
GLfloat noShininess = 0.0;
GLfloat highShininess = 100.0;