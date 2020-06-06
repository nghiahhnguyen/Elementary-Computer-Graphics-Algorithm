#include "shape.h"

#pragma once

GLfloat angleTorus = 0.0f;

class Torus : Object {
private:
	int sectorCount,
		innerSectorCount;
	double sectorStep,
		innerSectorStep,
		minorRadius,
		majorRadius;
	GLfloat angleRot;
	vector<Point> ringCenters;
	vector<vector<Point>> vertices;

	void calculateVertices()
	{
		float x, y, z, sectorAngle, innerSectorAngle;
		for (int i = 0; i < sectorCount; ++i) {
			sectorAngle = sectorStep * i;
			x = minorRadius * cosf(sectorAngle);
			y = 0;
			z = minorRadius * sinf(sectorAngle);
			ringCenters.push_back(Point(x, y, z));
			// for (int j = 0; j < sectorCount; ++j) {
			// 	innerSectorAngle = sectorStep * j;
			// 	x =
			// }
		}
	}

public:
	Torus(int sectorCnt, int innerSectorCnt, float R, float r, GLfloat angleRot)
		: sectorCount(sectorCnt), minorRadius(r), angleRot(angleRot), majorRadius(R), innerSectorCount(innerSectorCnt)
	{
		sectorStep = 2 * PI / sectorCount;
		innerSectorStep = 2 * PI / innerSectorCount;
		calculateVertices();
	};

	void draw()
	{
		glLoadIdentity();						 // Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);		 // Move right and into the screen
		glRotatef(angleTorus, 1.0f, 1.0f, 1.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		int i, j, k;
		double s, t, x, y, z, twopi;

		twopi = 2 * PI;
		Point high, low, prevHigh, prevLow;
		for (i = 0; i < sectorCount; i++) {
			// glBegin(GL_QUAD_STRIP);
			for (j = 0; j <= innerSectorCount; j++) {
				s = (i + 1) % sectorCount;
				t = j % innerSectorCount;

				x = (majorRadius + minorRadius * cos(s * sectorStep)) * cos(t * innerSectorStep);
				y = (majorRadius + minorRadius * cos(s * sectorStep)) * sin(t * innerSectorStep);
				z = minorRadius * sin(s * sectorStep);
				high = Point(x, y, z);

				s = (i) % sectorCount;
				t = j % innerSectorCount;

				x = (majorRadius + minorRadius * cos(s * sectorStep)) * cos(t * innerSectorStep);
				y = (majorRadius + minorRadius * cos(s * sectorStep)) * sin(t * innerSectorStep);
				z = minorRadius * sin(s * sectorStep);
				low = Point(x, y, z);
				// glVertex3f(x, y, z);

				if (i != 0) {
					mapTextureToQuad((i + j) % NUM_BMPS, prevLow, prevHigh, high, low);
				}
				prevHigh = high;
				prevLow = low;
			}
			// glEnd();
		}

		angleTorus += angleRot;
	}
};