#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include <freeglut.h>

#include "Texture.h"

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} vec3d;

typedef struct {
	GLfloat x;
	GLfloat y;
} vec2d;

typedef struct {
	int vertexIndex;	// Index of this vertex in the object's vertices array
	int texCoordIndex; // Index of the texture coordinate for this vertex in the object's texCoords array
	int normalIndex;	// Index of the normal for this vertex in the object's normals array
} meshObjectFacePoint;

typedef struct {
	int pointCount;
	meshObjectFacePoint *points;
} meshObjectFace;

typedef struct {
	int vertexCount;
	vec3d *vertices;
	int texCoordCount;
	vec2d *texCoords;
	int normalCount;
	vec3d *normals;
	int faceCount;
	meshObjectFace *faces;
} meshObject;

void initOBJ(void);
meshObject* loadMeshObject(char *fileName);
void renderMeshObject(meshObject *object);
void initMeshObjectFace(meshObjectFace *face, char* faceData, int faceDataLength);
void freeMeshObject(meshObject *object);

meshObject *tree1Leaf;
meshObject *tree1Trunk;

meshObject *tree2Leaf;
meshObject *tree2Trunk;

meshObject *tree3Leaf;
meshObject *tree3Trunk;

meshObject *house;
meshObject *apartment;
meshObject *building;