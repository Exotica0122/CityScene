#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <freeglut.h>

// Textures
GLuint icyTexture;
GLuint skyTexture;
GLuint asphaltTexture;
GLuint roadTexture;
GLuint apartmentTexture;

// Prototypes

int loadPPM(char* filename);
void initTexture(void);