#pragma once

#include <freeglut.h>

#include "DisplayList.h"

void drawBuilding(double r, double g, double b, double scale);
void drawTallBuilding(double r, double g, double b, double scale);
void drawDoor(void);
void drawWindows(double startY, double endY);
void drawWindow(void);