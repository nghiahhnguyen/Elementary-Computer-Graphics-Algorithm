#pragma once

#include "SOIL.h"
#include <GL/glut.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdio.h>
#include <string>

#define NUM_BMPS 7
#define INF 1e9
#define PI 3.14159265359

GLuint texture[NUM_BMPS];

using namespace std;

class Point {
public:
	Point(float x, float y, float z)
		: x(x), y(y), z(z){};
	Point()
		: x(0), y(0), z(0){};
	float x, y, z;

	Point &operator=(const Point &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	friend ostream &operator<<(ostream &out, const Point &point)
	{
		out << point.x << ' ' << point.y << ' ' << point.z << endl;
		return out;
	};
};

class Object {
public:
	virtual void draw() = 0;
};

vector<int> textureList(6);
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist(0, NUM_BMPS - 1);
void generateRandomTextureLoading()
{
	for (int i = 0; i < 6; ++i) {
		textureList[i] = dist(gen);
	}
}

int loadGLTextures() // Load Bitmaps And Convert To Textures
{
	for (int i = 0; i < NUM_BMPS; ++i) {
		/* load an image file directly as a new OpenGL texture */
		string path = "./img/" + to_string(i) + ".bmp";
		texture[i] = SOIL_load_OGL_texture(
			path.c_str(),
			SOIL_LOAD_RGB,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y);
		if (texture[i] == 0) {
			printf("Failed to load image %d\n", i);
			return false;
		}
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	return true;
}

void mapTextureToQuad(int textureIdx, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[textureIdx]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(1, 1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(1, 0);
	glVertex3f(x3, y3, z3);
	glTexCoord2f(0, 0);
	glVertex3f(x4, y4, z4);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void mapTextureToQuad(int textureIdx, Point a, Point b, Point c, Point d)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[textureIdx]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(a.x, a.y, a.z);
	glTexCoord2f(1, 1);
	glVertex3f(b.x, b.y, b.z);
	glTexCoord2f(1, 0);
	glVertex3f(c.x, c.y, c.z);
	glTexCoord2f(0, 0);
	glVertex3f(d.x, d.y, d.z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void mapTextureToTriangle(int textureIdx, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[textureIdx]);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 1);
	glVertex3f(x1, y1, z1);
	glTexCoord2f(1, 1);
	glVertex3f(x2, y2, z2);
	glTexCoord2f(1, 0);
	glVertex3f(x3, y3, z3);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}