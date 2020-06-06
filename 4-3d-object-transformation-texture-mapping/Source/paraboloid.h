#include "circle.h"
#include "shape.h"
#pragma once

class Paraboloid : public Circle {
protected:
	float a, b;
	vector<vector<Point>> verticesPara;
	GLfloat angleParaboloid = 0.0f;
	void calculateVertices()
	{
		glBegin(GL_POINTS);
		for (int i = 0; i < sectorCount; ++i) {
			float x, y, z, theta;
			for (float r = 0; r <= radius; r += 0.3) {
				theta = i * sectorStep;
				x = r * cosf(theta);
				z = r * sinf(theta);
				y = a * x * x + b * z * z;
				verticesPara[i].push_back(Point(x, y, z));
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}

public:
	Paraboloid(int sectorCnt, float r, float a, float b, GLfloat angleRot)
		: Circle(sectorCnt, r, angleRot), a(a), b(b)
	{
		verticesPara.assign(sectorCount, vector<Point>());
		// calculateVertices();
	};

	void draw()
	{
		calculateVertices();
		glLoadIdentity();							  // Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);			  // Move right and into the screen
		glRotatef(angleParaboloid, 0.0f, 1.0f, 0.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < sectorCount; ++i) {
			if (verticesPara.size() == 0) {
				cout << "No verticesPara to draw\n";
				break;
			}
			int length = verticesPara[0].size();
			for (int j = 0; j < length - 1; ++j) {
				int sector1 = i,
					sector2 = (i + 1) % sectorCount;
				Point a = verticesPara[sector1][j],
					  b = verticesPara[sector1][j + 1],
					  c = verticesPara[sector2][j],
					  d = verticesPara[sector2][j + 1];
				mapTextureToQuad((i + j) % NUM_BMPS, a, b, d, c);
			}
		}

		angleParaboloid += angleRot;
	}
};