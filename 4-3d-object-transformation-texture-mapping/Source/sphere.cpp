#include "shape.h"

GLfloat angle = 0.0f;

class Sphere : Object {
private:
	int sectorCount,
		stackCount,
		radius;
	float sectorStep,
		stackStep;
	GLfloat angleRot;
	vector<Point> vertices;

	void calculateVertices()
	{
		float x, y, z, rcos,
			sectorAngle, stackAngle;
		for (int i = 0; i <= stackCount; ++i) {
			stackAngle = PI / 2 - i * stackStep;
			rcos = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);
			for (int j = 0; j <= sectorCount; ++j) {
				sectorAngle = j * sectorStep;
				x = rcos * cosf(sectorAngle);
				y = rcos * sinf(sectorAngle);
				vertices.push_back(Point(x, y, z));
			}
		}
	}

public:
	Sphere(int sectorCnt, int stackCnt, int r, GLfloat angleRot)
		: sectorCount(sectorCnt), stackCount(stackCnt), radius(r), angleRot(angleRot)
	{
		sectorStep = 2 * PI / sectorCount;
		stackStep = PI / stackCount;
	};

	void draw()
	{
		calculateVertices();

		glLoadIdentity();					// Reset the model-view matrix
		glTranslatef(-1.5f, 0.0f, -7.0f);	// Move right and into the screen
		glRotatef(angle, 1.0f, 1.0f, 1.0f); // Rotate about (1,1,1)-axis
		glColor3f(1.0f, 1.0f, 1.0f);

		int k1, k2;
		for (int i = 0; i < stackCount; ++i) {
			k1 = i * (sectorCount + 1);
			k2 = k1 + sectorCount + 1;
			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				// points in CW order
				Point a = vertices[k1],
					  b = vertices[k1 + 1],
					  c = vertices[k2 + 1],
					  d = vertices[k2];
				if (i != 0) {
					mapTextureToTriangle(0, a.x, a.y, a.z, d.x, d.y, d.z, b.x, b.y, b.z);
				}
				if (i != (stackCount - 1)) {
					mapTextureToTriangle(1, b.x, b.y, b.z, d.x, d.y, d.z, c.x, c.y, c.z);
				}
			}
		}

		angle -= angleRot;
	};
};