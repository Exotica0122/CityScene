#pragma once

#include <freeglut.h>
#include "OBJ.h"
#include "Material.h"

// DisplayList
GLuint BuildingDL;

GLuint treeDL;

void initDisplayLists(void);
void createBuildingDisplayList(void);
void createTreeDisplayList(void);