#include "shape.h"
#pragma once

GLfloat angleHyperboloid = 0.0f;

class Hyperboloid : public Object {
protected:
	int sectorCount;
	float sectorStep, a, b, c;
	GLfloat angleRot;
	vector<vector<Point>> vertices;
	void calculateVertices()
	{
		for (int i = 0; i < sectorCount; ++i) {
			float x, y, z, theta, v, a = 1, b = 1, c = 1;
			for (float j = -1; j <= 1; j += 0.3) {
				theta = i * sectorStep;
				v = j;
				x = a * cosh(v) * cos(theta);
				y = c * sinh(v);
				z = b * cosh(v) * sin(theta);
				vertices[i].push_back(Point(x, y, z));
			}
		}
	}

public:
	Hyperboloid(int sectorCnt, float a, float b, float c, GLfloat angleRot)
		: sectorCount(sectorCnt), a(a), b(b), c(c), angleRot(angleRot)
	{
		sectorStep = 2 * PI / sectorCount;
		vertices.assign(sectorCount, vector<Point>());
		calculateVertices();
	};

	void draw()
	{
		// calculateVertices();
		glLoadIdentity();							   // Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);			   // Move right and into the screen
		glRotatef(angleHyperboloid, 0.0f, 1.0f, 0.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < sectorCount; ++i) {
			if (vertices.size() == 0) {
				cout << "No vertices to draw\n";
				break;
			}
			int length = vertices[0].size();
			for (int j = 0; j < length - 1; ++j) {
				int sector1 = i,
					sector2 = (i + 1) % sectorCount;
				Point a = vertices[sector1][j],
					  b = vertices[sector1][j + 1],
					  c = vertices[sector2][j],
					  d = vertices[sector2][j + 1];
				mapTextureToQuad((i + j) % NUM_BMPS, a, b, d, c);
			}
		}

		angleHyperboloid += angleRot;
	}
};