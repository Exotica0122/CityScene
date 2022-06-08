#pragma once
#include <freeglut.h>

#include "Material.h"
#include "Texture.h"

#define GROUND_WIDTH 500
#define GROUND_LENGTH 500
#define GROUND_GRID 10

#define SAND_GRID 3

#define ROAD_WIDTH 250
#define ROAD_LENGTH 10
#define ROAD_GRID 10

void basicGround(void);
void drawRoad(void);