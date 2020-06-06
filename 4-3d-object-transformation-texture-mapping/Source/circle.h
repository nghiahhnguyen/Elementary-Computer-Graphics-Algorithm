#include "shape.h"

#pragma once

class Circle : Object {
protected:
	int sectorCount,
		radius;
	float sectorStep;
	GLfloat angleRot,
		angleCircle = 0.0f;
	vector<Point> vertices;

	virtual void calculateVertices()
	{
		float x, y, z, sectorAngle;
		for (int i = 0; i < sectorCount; ++i) {
			sectorAngle = sectorStep * i;
			x = radius * cosf(sectorAngle);
			y = radius * sinf(sectorAngle);
			z = 0;
			vertices.push_back(Point(x, y, z));
		}
	}

public:
	Circle(int sectorCnt, int r, GLfloat angleRot)
		: sectorCount(sectorCnt), radius(r), angleRot(angleRot)
	{
		sectorStep = 2 * PI / sectorCount;
		calculateVertices();
	};

	virtual void draw()
	{
		glLoadIdentity();						  // Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);		  // Move right and into the screen
		glRotatef(angleCircle, 0.0f, 0.0f, 1.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < sectorCount; ++i) {
			Point a = vertices[i],
				  b = vertices[(i + 1) % int(vertices.size())];
			mapTextureToTriangle(i % NUM_BMPS, a.x, a.y, a.z, b.x, b.y, b.z, 0, 0, 0);
		}

		angleCircle -= angleRot;
	}
};