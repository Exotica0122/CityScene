#include "DisplayList.h"

void initDisplayLists(void)
{
	createBuildingDisplayList();
	createTreeDisplayList();
}

void createBuildingDisplayList(void)
{
	BuildingDL = glGenLists(3);

	glNewList(BuildingDL, GL_COMPILE);
	renderMeshObject(house);
	glEndList();

	glNewList(BuildingDL + 1, GL_COMPILE);
	glScaled(10, 10, 10);
	renderMeshObject(building);
	glEndList();

	glNewList(BuildingDL + 2, GL_COMPILE);
	glPushMatrix();
	glScaled(70, 70, 70);
	renderMeshObject(apartment);
	glPopMatrix();
	glEndList();
}

void createTreeDisplayList(void)
{
	treeDL = glGenLists(3);

	// -------------------------------------------------------

	glNewList(treeDL, GL_COMPILE);
	glPushMatrix();
	glScaled(0.09, 0.09, 0.09);

	glMaterialfv(GL_FRONT, GL_AMBIENT, treeLeafMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, treeLeafMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, treeLeafMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, noShininess);

	renderMeshObject(tree1Leaf);

	glMaterialfv(GL_FRONT, GL_AMBIENT, treeTrunkMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, treeTrunkMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, treeTrunkMaterial);

	renderMeshObject(tree1Trunk);

	glPopMatrix();
	glEndList();

	// -------------------------------------------------------

	glNewList(treeDL + 1, GL_COMPILE);
	glPushMatrix();
	glScaled(3, 3, 3);

	glMaterialfv(GL_FRONT, GL_AMBIENT, treeLeafMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, treeLeafMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, treeLeafMaterial);

	renderMeshObject(tree2Leaf);

	glMaterialfv(GL_FRONT, GL_AMBIENT, treeTrunkMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, treeTrunkMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, treeTrunkMaterial);

	renderMeshObject(tree2Trunk);
	glPopMatrix();
	glEndList();
	
	// -------------------------------------------------------

	glNewList(treeDL + 2, GL_COMPILE);
	glPushMatrix();
	glScaled(1.5, 1.5, 1.5);

	glMaterialfv(GL_FRONT, GL_AMBIENT, treeLeafMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, treeLeafMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, treeLeafMaterial);

	renderMeshObject(tree3Leaf);

	glMaterialfv(GL_FRONT, GL_AMBIENT, treeTrunkMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, treeTrunkMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, treeTrunkMaterial);

	renderMeshObject(tree3Trunk);
	glPopMatrix();
	glEndList();
}