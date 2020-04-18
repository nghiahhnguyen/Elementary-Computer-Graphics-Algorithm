#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>

using namespace std;

class Shape {
protected:
	stringstream ss;

public:
	Shape(){};
	~Shape(){};
	void convert(ifstream &fin)
	{
		string line;
		getline(fin, line);
		ss = stringstream(line);
	}
	virtual void readInput() = 0;
	virtual void draw() = 0;
};

class Line : public Shape {
private:
	int x1, y1, x2, y2, option;

public:
	// Line(int a, int b, int c, int d)
	// 	: x1(a), y1(b), x2(c), y2(d){};
	Line(){};
	~Line(){};

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

	void readInput()
	{
		ss >> option >> x1 >> y1 >> x2 >> y2;
	}

	void draw()
	{
		glBegin(GL_POINTS);
		if (option == 0) {
			drawLineDDA();
		}
		else {
			drawLineBresenham();
		}
		glEnd();
	}
};

class Circle : public Shape {
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
	Circle(){};
	~Circle(){};

	void readInput()
	{
		ss >> xt >> yt >> r;
	}

	void draw()
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

class Ellipse {
private:
	int xt, yt, a, b;

public:
	Ellipse(int xt, int yt, int a, int b)
		: xt(xt), yt(yt), a(a), b(b) {}
};

class Parabola {
private:
	int xt, yt, p;

public:
};

class Hyperbola {
private:
	int xt, yt, a, b;

public:
};

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);

	ifstream fin;
	fin.open("INP.txt");
	string geometricShape;
	Shape *shape = NULL;
	while (getline(fin, geometricShape)) {
		if (shape != NULL)
			delete shape;

		if (geometricShape == "line") {
			shape = new Line();
		}
		else {
			shape = new Circle();
		}

		shape->convert(fin);
		shape->readInput();
		auto start = chrono::high_resolution_clock::now();
		shape->draw();
		auto end = chrono::high_resolution_clock::now();
		cout << "The time to draw a/an " << geometricShape << " is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";
	}

	fin.close();

	glFlush();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);   // set the size of the window
	glutInitWindowPosition(10, 10); // the position of the top-left of window
	glutCreateWindow("OpenGL Drawing shapes");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutMainLoop();

	return 0;
}