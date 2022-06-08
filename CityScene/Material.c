#include "Material.h"

//  position the light source 
//GLfloat lightPosition[] = { 10.0, 10.0, 10.0, 1.0 };

// a material that is all zeros
GLfloat zeroMaterial[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat midMaterial[] = { 0.5, 0.5, 0.5, 1 };

// a red ambient material
GLfloat redAmbient[] = { 0.5, 0.0, 0.0, 1.0 };

GLfloat whiteAmbient[] = {1, 1, 1, 1};

// a blue diffuse material
GLfloat blueDiffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };

// a red diffuse material
GLfloat redDiffuse[] = { 1.0, 0.0, 0.0, 1.0 };

GLfloat whiteDiffuse[] = {1, 1, 1, 1};

// a white specular material
GLfloat whiteSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

// the degrees of shinnines (size of the specular highlight, bigger number means smaller highlight)
GLfloat noShininess = 0.0;
GLfloat midShininess = 50.0;
GLfloat highShininess = 100.0;

// Lighthouse light

GLfloat lightAmbient[] = { 1, 1, 0, 0.5 };
GLfloat lightDiffuse[] = { 1, 1, 0, 0.5 };
GLfloat lightSpecular[] = { 1, 1, 0, 0.5 };

GLfloat emissive[] = { 1, 1, 1, 1.0 };

// Tree 1 Material

GLfloat treeLeafMaterial[] = {0, 0.5, 0, 1};
GLfloat treeTrunkMaterial[] = {0.65, 0.16, 0.16, 1};