#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <math.h>

using namespace std;

class Line {
private:
	int x1, y1, x2, y2;

public:
	Line(int a, int b, int c, int d)
		: x1(a), y1(b), x2(c), y2(d){};

	void drawLineDDA()
	{
		double dy = y2 - y1,
			   dx = x2 - x1;
		double m = 1.0 * dy / dx;
		double x = x1, y = y1;
		while (x <= x2) {
			glVertex3f(x, round(y), 0);
			++x;
			y += m;
		}
	}

	void drawLineBresenham()
	{
		int dy = y2 - y1,
			dx = x2 - x1,
			error = 2 * dy - dx,
			x = x1,
			y = y1;
		glVertex3i(x, y, 0);
		while (x <= x2) {
			if (error < 0) {
				error += 2 * dy;
			}
			else {
				error += 2 * (dy - dx);
				++y;
			}
			++x;
			glVertex3i(x, y, 0);
		}
	}
};


void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glClearColor(1.0, 0.0, 0.0, 1.0); //clear red
	glLineWidth(2.5);
	Line line(0, 0, 3, 2);
	// Line
	auto start = chrono::high_resolution_clock::now();
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	line.drawLineDDA();
	glEnd();
	auto end = chrono::high_resolution_clock::now();
	cout << "The time to draw a line with DDA is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";


	start = chrono::high_resolution_clock::now();
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0);
	line.drawLineBresenham();
	glEnd();
	end = chrono::high_resolution_clock::now();
	cout << "The time to draw a line with Bresenham algorithm is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";

	glutSwapBuffers();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL First Window");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutMainLoop();

	return 0;
}