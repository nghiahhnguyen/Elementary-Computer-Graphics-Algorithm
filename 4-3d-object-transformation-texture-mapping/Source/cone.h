#include "shape.h"

#pragma once

GLfloat angleCone = 0.0f;

class Cone : Object {
private:
	int sectorCount,
		radius;
	float height, sectorStep;
	GLfloat angleRot;
	vector<Point> vertices;

	void calculateVertices()
	{
		float x, y, z, sectorAngle;
		for (int i = 0; i < sectorCount; ++i) {
			sectorAngle = sectorStep * i;
			x = radius * cosf(sectorAngle);
			y = 0;
			z = radius * sinf(sectorAngle);
			vertices.push_back(Point(x, y, z));
		}
	}

public:
	Cone(int sectorCnt, int r, float height, GLfloat angleRot)
		: sectorCount(sectorCnt), radius(r), angleRot(angleRot), height(height)
	{
		sectorStep = 2 * PI / sectorCount;
		calculateVertices();
	};

	void draw()
	{
		glLoadIdentity();						  // Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);		  // Move right and into the screen
		glRotatef(-4 * PI, 1.0f, 0.0f, 0.0f);
		glRotatef(angleCone, 0.0f, 1.0f, 0.0f); // Rotate about (0, 1, 0)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < sectorCount; ++i) {
			Point a = vertices[i],
				  b = vertices[(i + 1) % int(vertices.size())];
			mapTextureToTriangle(i % NUM_BMPS, a.x, a.y, a.z, b.x, b.y, b.z, 0, 0, 0);
			mapTextureToTriangle((i + 1) % NUM_BMPS, a.x, a.y, a.z, b.x, b.y, b.z, 0, height, 0);
		}

		angleCone += angleRot;
	}
};