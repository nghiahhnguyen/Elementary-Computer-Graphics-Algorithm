#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>

using namespace std;

class Point {
private:
	int x, y;

public:
	Point(){};
	Point(int x, int y)
		: x(x), y(y){};
	int getX() { return x; }
	int getY() { return y; }
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	bool operator<(Point other)
	{
		return x < other.x || (x == other.x && y < other.y);
	}
};

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

	unsigned long long calculateDistanceVector(vector<Point> l1, vector<Point> l2)
	{
		sort(l1.begin(), l1.end());
		sort(l2.begin(), l2.end());
		int length = min(int(l1.size()), int(l2.size()));
		unsigned long long loss = 0;
		for (int i = 0; i < length; ++i) {
			int y1 = l1[i].getY(), y2 = l2[i].getY(), x1 = l1[i].getX(), x2 = l2[i].getX();
			double singleLoss = sqrt(((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
			// printf("%d %d %d %d\n", x1, y1, x2, y2);
			// printf("%lf\n", singleLoss);
			loss += singleLoss;
		}
		return loss;
	}
	virtual void readInput() = 0;
	virtual void draw() = 0;
    virtual void drawOpenGL(vector<int>& results) = 0;

    // calculate the loss between my implementation and OpenGL
	void calculateDistance(vector<Point> pointsList)
	{

		u_char testColor[3] = {255, 255, 255};
		glColor3f(testColor[0], testColor[1], testColor[2]);

		vector<int> params;
		drawOpenGL(params);
		int testX1 = params[0],
			testY1 = params[1],
			testX2 = params[2],
			testY2 = params[3];

		// dynamically allocate buffer for glReadPixels
		int height = testY2 - testY1, width = testX2 - testX1;
		int bufferSize = height * width * 3; // 3 for RGB channels
		u_char *buffer = new u_char[bufferSize];

		// start reading
		glReadPixels(testX1, testY1, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		vector<Point> correctPointsList;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				bool isCorrectPoint = true;
				for (int channel = 0; channel < 3; ++channel) {
					u_char color = buffer[i * width + j + channel];
					if (color != testColor[channel])
						isCorrectPoint = false;
				}
				if (isCorrectPoint) {
                    Point correctPoint = Point(i, j);
					correctPointsList.push_back(correctPoint);
                    printf("%d %d\n", correctPoint.getX(), correctPoint.getY());
				}
			}
		}

		// printf("%d %d\n", int(pointsList.size()), int(correctPointsList.size()));

		if (int(pointsList.size()) != 0 && int(correctPointsList.size()) != 0) {
			unsigned long long loss = calculateDistanceVector(pointsList, correctPointsList);
			cout << "The distance is " << loss << endl;
		}

		// delete buffer
		if (buffer == NULL)
			delete[] buffer;
	}
};

class Line : public Shape {
private:
	int x1, y1, x2, y2, option;

public:
	// Line(int a, int b, int c, int d)
	// 	: x1(a), y1(b), x2(c), y2(d){};
	Line(){};
	~Line(){};

	vector<Point> drawLineDDA()
	{
		double dy = y2 - y1,
			   dx = x2 - x1;
		double m = 1.0 * dy / dx;
		double x = x1, y = y1;
		vector<Point> pointsList;
		while (x <= x2) {
			glVertex3f(x, round(y), 0);
			pointsList.push_back(Point(x, round(y)));
			++x;
			y += m;
		}
		return pointsList;
	}

	void drawLineBresenham()
	{
		int dy = y2 - y1,
			dx = x2 - x1,
			error = 2 * dy - dx,
			x = x1,
			y = y1;
		glVertex2i(x, y);
		while (x <= x2) {
			if (error < 0) {
				error += 2 * dy;
			}
			else {
				error += 2 * (dy - dx);
				++y;
			}
			++x;
			glVertex2i(x, y);
		}
	}

	void readInput()
	{
		ss >> option >> x1 >> y1 >> x2 >> y2;
	}

	// draw the OpenGL implementation
	void drawOpenGL(vector<int> &results)
	{
		// move the test image above the line
		int offset = y2 - y1 + 1;
		int testX1 = x1, testY1 = y1 + offset, testX2 = x2, testY2 = y2 + offset;

		glBegin(GL_LINES);
		glVertex2f(testX1, testY1);
		glVertex2f(testX2, testY2);
		glEnd();

		results.push_back(testX1);
		results.push_back(testY1);
		results.push_back(testX2);
		results.push_back(testY2);
	}

	void draw()
	{
		glBegin(GL_POINTS);
		vector<Point> pointsList;
		if (option == 0) {
			pointsList = drawLineDDA();
		}
		else {
			drawLineBresenham();
		}
		glEnd();
		// calculateDistance(pointsList);
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

    // draw the OpenGL implementation
	void drawOpenGL(vector<int> &results)
	{
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