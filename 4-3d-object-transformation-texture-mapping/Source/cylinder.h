#include "shape.h"

#pragma once

GLfloat angleCylinder = 0.0f;

class Cylinder : Object {
protected:
	int sectorCount,
		radius;
	float sectorStep, height;
	GLfloat angleRot;
	vector<Point> verticesBottom, verticesTop;

	virtual void calculateVertices()
	{
		float x, y, z, sectorAngle;
		for (int i = 0; i < sectorCount; ++i) {
			sectorAngle = sectorStep * i;
			x = radius * cosf(sectorAngle);
			y = 0;
			z = radius * sinf(sectorAngle);
			verticesBottom.push_back(Point(x, y, z));
			verticesTop.push_back(Point(x, height, z));
		}
	}

public:
	Cylinder(int sectorCnt, int r, int height, GLfloat angleRot)
		: sectorCount(sectorCnt), radius(r), angleRot(angleRot), height(height)
	{
		sectorStep = 2 * PI / sectorCount;
		calculateVertices();
	};

	void draw()
	{
		glLoadIdentity();							// Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);			// Move right and into the screen
		glRotatef(angleCylinder, 1.0f, 1.0f, 1.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < sectorCount; ++i) {
			Point a = verticesBottom[i],
				  b = verticesBottom[(i + 1) % int(verticesBottom.size())];
			mapTextureToTriangle(i % NUM_BMPS, a.x, a.y, a.z, b.x, b.y, b.z, 0, 0, 0);
			Point c = verticesTop[i],
				  d = verticesTop[(i + 1) % int(verticesTop.size())];
			mapTextureToTriangle((i + 1) % NUM_BMPS, c.x, c.y, c.z, d.x, d.y, d.z, 0, height, 0);
			mapTextureToQuad((i + 2) % NUM_BMPS, a.x, a.y, a.z, b.x, b.y, b.z, d.x, d.y, d.z, c.x, c.y, c.z);
		}

		angleCylinder -= angleRot;
	}
};