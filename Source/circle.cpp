#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <math.h>

using namespace std;

class Circle {
private:
	int xt, yt, r;

	void plot(int x, int y)
	{
		glVertex2i(x, y);
	}

	void drawCorrespondingPoints(int x, int y)
	{
		glBegin(GL_POINTS);
		plot(xt + x, yt + y);
		plot(xt + y, yt + x);
		plot(xt + y, yt - x);
		plot(xt + x, yt - y);
		plot(xt - x, yt - y);
		plot(xt - y, yt - x);
		plot(xt - y, yt + x);
		plot(xt - x, yt + y);
		glEnd();
	}

public:
	Circle(int a, int b, int c)
		: xt(a), yt(b), r(c){};

	void drawMidPoint()
	{
		int x = 0,
			y = r,
			p = 1 - r;

		// plot(x + xt, y + yt);
		while (x < y) {
			++x;
			if (p < 0) {
				p = p + 2 * x + 1;
			}
			else {
				--y;
				p = p + 2 * x - 2 * y + 1;
			}
			drawCorrespondingPoints(x, y);
		}
	}
};

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);

	Circle circle(200, 300, 100);
	// Line
	auto start = chrono::high_resolution_clock::now();
	// glBegin(GL_LINE_SMOOTH);
	circle.drawMidPoint();
	// glEnd();
	auto end = chrono::high_resolution_clock::now();
	cout << "The time to draw a circle with MidPoint algorithm is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";

	glFlush();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);   // set the size of the window
	glutInitWindowPosition(10, 10); // the position of the top-left of window
	glutCreateWindow("OpenGL First Window");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutMainLoop();

	return 0;
}