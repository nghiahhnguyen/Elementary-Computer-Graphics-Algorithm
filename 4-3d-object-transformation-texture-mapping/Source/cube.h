#include "shape.h"

// #pragma once

class Cube {
protected:
	float sideLength;
	GLfloat angleCube = 0.0f; // Rotational angle for cube
public:
	Cube(float sideLength)
		: sideLength(sideLength){};
	virtual void draw()
	{
		// Render a color-cube consisting of 6 quads with different colors
		glLoadIdentity();						// Reset the model-view matrix
		glTranslatef(1.5f, 0.0f, -7.0f);		// Move right and into the screen
		glRotatef(angleCube, 0.0f, 1.0f, 0.0f); // Rotate about (1,1,1)-axis [NEW]

		// Top face (y = 1.0f)
		// Define vertices in counter-clockwise (CCW) order with normal pointing out
		mapTextureToQuad(textureList[0], sideLength, sideLength, -sideLength, -sideLength, sideLength, -sideLength, -sideLength, sideLength, sideLength, sideLength, sideLength, sideLength);

		// Bottom face (y = -sideLength)
		mapTextureToQuad(textureList[1], sideLength, -sideLength, sideLength, -sideLength, -sideLength, sideLength, -sideLength, -sideLength, -sideLength, sideLength, -sideLength, -sideLength);

		// Front face (z = sideLength)
		mapTextureToQuad(textureList[2], sideLength, sideLength, sideLength, -sideLength, sideLength, sideLength, -sideLength, -sideLength, sideLength, sideLength, -sideLength, sideLength);

		// Back face (z = -sideLength)
		mapTextureToQuad(textureList[3], sideLength, -sideLength, -sideLength, -sideLength, -sideLength, -sideLength, -sideLength, sideLength, -sideLength, sideLength, sideLength, -sideLength);

		// Left face (x = -sideLength)
		mapTextureToQuad(textureList[4], -sideLength, sideLength, sideLength, -sideLength, sideLength, -sideLength, -sideLength, -sideLength, -sideLength, -sideLength, -sideLength, sideLength);

		// Right face (x = sideLength)
		mapTextureToQuad(textureList[5], sideLength, sideLength, -sideLength, sideLength, sideLength, sideLength, sideLength, -sideLength, sideLength, sideLength, -sideLength, -sideLength);

		angleCube += 0.8f;
	}
};