#include "shape.h"

// #pragma once

class Cube {
protected:
	int sideLength;
	GLfloat angleCube = 0.0f; // Rotational angle for cube
public:
	Cube(int sideLength)
		: sideLength(sideLength){};
	virtual void draw()
	{
		// Render a color-cube consisting of 6 quads with different colors
		glLoadIdentity();						// Reset the model-view matrix
		glTranslatef(1.5f, 0.0f, -7.0f);		// Move right and into the screen
		glRotatef(angleCube, 0.0f, 1.0f, 0.0f); // Rotate about (1,1,1)-axis [NEW]

		// Top face (y = 1.0f)
		// Define vertices in counter-clockwise (CCW) order with normal pointing out
		mapTextureToQuad(textureList[0], 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		// Bottom face (y = -1.0f)
		mapTextureToQuad(textureList[1], 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f);

		// Front face (z = 1.0f)
		mapTextureToQuad(textureList[2], 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f);

		// Back face (z = -1.0f)
		mapTextureToQuad(textureList[3], 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);

		// Left face (x = -1.0f)
		mapTextureToQuad(textureList[4], -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f);

		// Right face (x = 1.0f)
		mapTextureToQuad(textureList[5], 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f);

		angleCube += 0.8f;
	}
};