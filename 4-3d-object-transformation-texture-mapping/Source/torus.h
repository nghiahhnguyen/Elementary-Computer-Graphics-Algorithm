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

public:
	Torus(int sectorCnt, int innerSectorCnt, float R, float r, GLfloat angleRot)
		: sectorCount(sectorCnt), minorRadius(r), angleRot(angleRot), majorRadius(R), innerSectorCount(innerSectorCnt)
	{
		sectorStep = 2 * PI / sectorCount;
		innerSectorStep = 2 * PI / innerSectorCount;
	};

	void draw()
	{
		glLoadIdentity();						 // Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);		 // Move right and into the screen
		glRotatef(angleTorus, 0.0f, 0.0f, 1.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		int i, j, k;
		double s, t, x, y, z, twopi;

		twopi = 2 * PI;
		Point high, low, prevHigh, prevLow;
		// for each ring
		for (i = 0; i < sectorCount; i++) {
			// for pair of points on the ring
			for (j = 0; j <= innerSectorCount; j++) {
				// calculate 2 points
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

				// draw a quad between these two points and the points from the previous iteration of j
				if (i != 0) {
					mapTextureToQuad((i + j) % NUM_BMPS, prevLow, prevHigh, high, low);
				}
				prevHigh = high;
				prevLow = low;
			}
		}

		angleTorus -= angleRot;
	}
};