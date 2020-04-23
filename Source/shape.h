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
	int option = 0,
		verticalOffset;

public:
	Shape(){};
	~Shape(){};
	virtual void readInput() = 0;
	virtual void draw() = 0;
	virtual void drawOpenGL(vector<int> &results) = 0;

	bool getOption() { return option; }

	void drawOpenGLWithClock(vector<int> &results)
	{
		auto start = chrono::high_resolution_clock::now();
		this->drawOpenGL(results);
		auto end = chrono::high_resolution_clock::now();
		cout << "The time to draw that shape using OpenGL implementation is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";
	}

	void convert(ifstream &fin)
	{
		string line;
		getline(fin, line);
		ss = stringstream(line);
		ss >> option;
	}

	unsigned long long calculateDistanceVector(vector<Point> l1, vector<Point> l2)
	{
		sort(l1.begin(), l1.end());
		sort(l2.begin(), l2.end());
		printf("Size of the points vectors: %d %d\n", int(l1.size()), int(l2.size()));
		int length = min(int(l1.size()), int(l2.size()));
		unsigned long long loss = 0;
		for (int i = 0; i < length; ++i) {
			int y1 = l1[i].getY() - verticalOffset, y2 = l2[i].getY() - verticalOffset, x1 = l1[i].getX(), x2 = l2[i].getX();
			double singleLoss = sqrt(((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
			loss += singleLoss;
		}
		return loss;
	}

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
		verticalOffset = y2 - y1 + 5;
		int testX1 = x1, testY1 = y1 + verticalOffset, testX2 = x2, testY2 = y2 + verticalOffset;

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

class Ellipse : public Shape {
private:
	int xt, yt, a, b;

public:
	Ellipse() {}
	Ellipse(int xt, int yt, int a, int b)
		: xt(xt), yt(yt), a(a), b(b) {}

	void readInput()
	{
		ss >> xt >> yt >> a >> b;
	}

	void drawCorrespondingPoints(int x, int y)
	{
		glBegin(GL_POINTS);
		glVertex2i(xt + x, yt + y);
		glVertex2i(xt + x, yt - y);
		glVertex2i(xt - x, yt + y);
		glVertex2i(xt - x, yt - y);
		glEnd();
	}

	void draw()
	{
		int x = 0, y = b;
		double dx = 2 * b * b * x,
			   dy = 2 * a * a * y,
			   p1 = b * b - a * a * b + 0.25 * a * a;

		// run for region 1 first (dx/dy < 1)
		while (dx < dy) {
			drawCorrespondingPoints(x, y);
			if (p1 >= 0) {
				++x, --y;
				dx += (2 * b * b);
				dy -= (2 * a * a);
				p1 += (dx - dy + b * b);
			}
			else {
				++x;
				dx += (2 * b * b);
				p1 += (dx + b * b);
			}
		}

		// then run for region 2 (dx/dy >= 1)
		double p2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
		while (y >= 0) {
			drawCorrespondingPoints(x, y);
			if (p2 <= 0) {
				--y, ++x;
				dx += (2 * b * b);
				dy -= (2 * a * a);
				p2 += (dx - dy + a * a);
			}
			else {
				--y;
				dy -= (2 * a * a);
				p2 += (a * a - dy);
			}
		}
	}

	void drawOpenGL(vector<int> &results){};
};

// Suppose that the parabola is symmetric around the X-axis
// The equation is y^2 = 2px, let error = y^2 - 2 px
class Parabola : public Shape {
private:
	int xt, yt, p;
	int range = 100; // the range of x for one side of the parabola

public:
	void readInput()
	{
		ss >> xt >> yt >> p;
	}

	void drawCorrespondingPoints(int x, int y)
	{
		glBegin(GL_POINTS);
		glVertex2i(xt + x, yt + y);
		glVertex2i(xt + x, yt - y);
		glEnd();
	}

	void draw()
	{
		int d = 1 - p;
		int x = 0,
			y = 0,
			p2 = 2 * p,
			p4 = 2 * p2;

		// first region
		while (y < p && x <= range) {
			drawCorrespondingPoints(x, y);
			if (d >= 0) {
				++x;
				d -= p2;
			}
			++y;
			d += (2 * y + 1);
		}

		// second region
		while (x <= range) {
			drawCorrespondingPoints(x, y);
			if (d <= 0) {
				++y;
				d += 4 * y;
			}
			x++;
			d -= p4;
		}
	}

	void drawOpenGL(vector<int> &results){};
};

class Hyperbola : public Shape {
private:
	int xt, yt, a, b,
		range = 50;

public:
	void readInput()
	{
		ss >> xt >> yt >> a >> b;
	}

	void drawCorrespondingPoints(int x, int y)
	{
		glBegin(GL_POINTS);
		glVertex2i(xt + x, yt + y);
		glVertex2i(xt + x, yt - y);
		glVertex2i(xt - x, yt + y);
		glVertex2i(xt - x, yt - y);
		glEnd();
	}

	void draw()
	{
		const int sqrA = a * a,
				  sqrB = b * b,
				  twoSqrA = sqrA << 1,
				  twoSqrB = sqrB << 1,
				  fourSqrA = twoSqrA << 1,
				  fourSqrB = twoSqrB << 1,
				  halfSqrA = sqrA >> 1,
				  halfSqrB = sqrB >> 1;
		int x = a,
			y = 0,
			dx = fourSqrB * (x + 1),
			dy = fourSqrA,
			p = twoSqrA - sqrB * (1 + 2 * a) + halfSqrB;

		while (p < dx and y <= range) {
			drawCorrespondingPoints(x, y);
			if (p >= 0) {
				p -= dx;
				++x;
				dx += fourSqrB;
			}
			p += (twoSqrA + dy);
			++y;
			dy += fourSqrA;
		}

		p = p - (dx + dy) >> 1 + sqrA + sqrB - halfSqrA - halfSqrB;
		// region 2
		if (a > b)
			while (y <= range) {
				drawCorrespondingPoints(x, y);
				if (p <= 0) {
					p += dy;
					++y;
					dy += fourSqrA;
				}
				p = p - twoSqrB - dx;
				++x;
				dx += fourSqrB;
			}
	}

	void drawOpenGL(vector<int> &results){};
};