#include "SOIL.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <random>

#define NUM_BMPS 7
#define INF 1e9
GLuint texture[NUM_BMPS];

using namespace std;

vector<int> textureList(6);

void generateRandomTextureLoading() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0, NUM_BMPS - 1);
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

void mapTextureToSurface(int textureIdx, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4 = INF, GLfloat y4 = INF, GLfloat z4 = INF)
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
	if (x4 != INF) {
		glTexCoord2f(0, 0);
		glVertex3f(x4, y4, z4);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}