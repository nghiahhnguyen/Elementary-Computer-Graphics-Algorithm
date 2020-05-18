#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <vector>

using namespace std;

const int WIDTH = 1200, HEIGHT = 800, PI = 3.14159265;

enum TRANSFORMATION {
	ROTATE_LEFT,
	ROTATE_RIGHT,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	ENLARGE,
	SHRINK
};

long long square(int x)
{
	return 1LL * x * x;
}

class RGBColor {
public:
	unsigned char r, g, b;
	RGBColor()
		: r(255), g(255), b(255){};
	RGBColor(const RGBColor &other)
	{
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
	}
	RGBColor(unsigned char r, unsigned char g, unsigned b)
		: r(r), g(g), b(b){};

	RGBColor &operator=(const RGBColor &other)
	{
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		return *this;
	}

	bool operator==(const RGBColor &other)
	{
		return r == other.r && g == other.g && b == other.b;
	}

	bool operator!=(const RGBColor &other)
	{
		return !(*this == other);
	}
};

RGBColor black = RGBColor(0, 0, 0);

ostream &operator<<(ostream &os, const RGBColor &color)
{
	os << int(color.r) << '/' << int(color.g) << '/' << int(color.b);
	return os;
}

RGBColor bitMap[WIDTH + 1][HEIGHT + 1];

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
	// stringstream ss;
	RGBColor color = RGBColor(0, 0, 0);
	int option = 0,
		verticalOffset = 100;

	void drawLineGLLine(int startX, int startY, int endX, int endY)
	{
		glBegin(GL_LINES);
		glVertex2i(startX, startY);
		glVertex2i(endX, endY);
		glEnd();
	}

	void plot(int x, int y)
	{
		// glColor3ub(color.r, color.g, color.b);
		// glVertex2i(x, y);
		// glFlush();
		// cout << "==\n"
		// << bitMap[x][y] << endl;
		// cout << color << endl;
		bitMap[x][y] = color;
		// cout << bitMap[x][y] << "\n\n";
	}

	void plot(int x, int y, RGBColor drawColor)
	{
		bitMap[x][y] = drawColor;
	}

public:
	Shape(){};
	~Shape(){};
	Shape &operator=(Shape &other)
	{
		this->color = other.color;
		this->option = other.option;
		this->verticalOffset = other.verticalOffset;
		return *this;
	}
	virtual void readInput(ifstream &fin) = 0;
	virtual void draw() = 0;
	virtual void drawOpenGL(vector<int> &results) = 0;
	virtual void updateVertices(vector<Point> &vertices) = 0;

	bool getOption() { return option; }

	void drawOpenGLWithClock(vector<int> &results)
	{
		u_char testColor[3] = {255, 255, 255};
		glColor3f(testColor[0], testColor[1], testColor[2]);
		auto start = chrono::high_resolution_clock::now();
		this->drawOpenGL(results);
		auto end = chrono::high_resolution_clock::now();
		cout << "The time to draw this shape using OpenGL implementation is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";
	}

	double calculateDistanceVector(vector<Point> l1, vector<Point> l2)
	{
		printf("Reach calculateDistanceVector\n");
		sort(l1.begin(), l1.end());
		sort(l2.begin(), l2.end());
		printf("Size of the points vectors: %d %d\n", int(l1.size()), int(l2.size()));
		int length = min(int(l1.size()), int(l2.size()));
		unsigned long long loss = 0;
		for (int i = 0; i < length; ++i) {
			int y1 = l1[i].getY() - verticalOffset, y2 = l2[i].getY() - verticalOffset, x1 = l1[i].getX(), x2 = l2[i].getX();
			double singleLoss = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
			loss += singleLoss;
		}
		return sqrt(loss);
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
		int height = testY2 - testY1 + 1, width = testX2 - testX1 + 1;
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

		if (int(pointsList.size()) != 0 && int(correctPointsList.size()) != 0) {
			double loss = calculateDistanceVector(pointsList, correctPointsList);
			cout << "The distance is " << loss << endl;
		}

		// delete buffer
		if (buffer == NULL)
			delete[] buffer;
	}

	virtual bool inside(int x, int y) { return true; };
	virtual double distanceToCenter(int x, int y) { return 0.0; };
	virtual void scanLineColoring(RGBColor fillingColor){};
	virtual void unSelect(){};
	virtual void switchToSelected(RGBColor focusColor){};
	virtual void transform(int option){};
};

class Line : public Shape {
private:
	int x1, y1, x2, y2, option = 0;

public:
	Line(int a, int b, int c, int d, int option = 0)
		: x1(a), y1(b), x2(c), y2(d), option(option){};
	Line(int a, int b, int c, int d, RGBColor color, int option = 0)
		: x1(a), y1(b), x2(c), y2(d), option(option)
	{
		this->color = color;
	};
	Line(){};
	Line &operator=(const Line &other)
	{
		x1 = other.x1;
		y1 = other.y1;
		x2 = other.x2;
		y2 = other.y2;
		option = other.option;
		return *this;
	}
	Line(int option)
		: option(option){};
	~Line(){};

	vector<Point> drawLineDDA()
	{
		double dy = y2 - y1,
			   dx = x2 - x1;

		vector<Point> pointsList;
		double x = x1, y = y1;
		// vertical line
		if (dy == 0) {
			while (x <= x2) {
				plot(x, y);
				++x;
			}
			return pointsList;
		}
		// horizontal line
		else if (dx == 0) {
			y = min(y1, y2);
			double end = max(y1, y2);
			while (y <= end) {
				plot(x, y);
				++y;
			}
			return pointsList;
		}
		double m = 1.0 * dy / dx; // slope
		cout << m << endl;
		if (abs(m) > 0 && abs(m) < 1) {
			double newM = abs(m);
			while (x <= x2) {
				int xC, yC;
				if (m > 0)
					xC = x, yC = round(y);
				else
					xC = x, yC = 2 * y1 - round(y);
				plot(xC, yC);
				++x;
				y += newM;
			}
		}
		else if (m > 1) {
			double newM = abs(1.0 * dx / dy);
			while (y <= y2) {
				int xC, yC;
				xC = round(x), yC = y;
				plot(xC, yC);
				++y;
				x += newM;
			}
		}
		else if (m < -1) {
			double newM = abs(1.0 * dx / dy);
			double end = 2 * y1 - y2;
			while (y <= end) {
				int xC, yC;
				xC = round(x), yC = 2 * y1 - y;
				plot(xC, yC);
				++y;
				x += newM;
			}
		}
		return pointsList;
	}

	vector<Point> drawLineBresenham()
	{
		vector<Point> pointsList;
		int dy = y2 - y1,
			dx = x2 - x1,
			error = 2 * dy - dx,
			x = x1,
			y = y1;
		while (x <= x2) {
			glVertex2i(x, y);
			if (error < 0) {
				error += 2 * dy;
			}
			else {
				error += 2 * (dy - dx);
				++y;
			}
			++x;
		}
		return pointsList;
	}

	void readInput(ifstream &fin)
	{
		fin >> x1 >> y1 >> x2 >> y2;
	}

	// draw the OpenGL implementation
	void drawOpenGL(vector<int> &results)
	{
		// move the test image above the line
		verticalOffset = y2 - y1 + 100;
		int testX1 = x1, testY1 = y1 + verticalOffset, testX2 = x2, testY2 = y2 + verticalOffset;

		glBegin(GL_LINES);
		glVertex2i(testX1, testY1);
		glVertex2i(testX2, testY2);
		glEnd();

		results.push_back(testX1);
		results.push_back(testY1);
		results.push_back(testX2);
		results.push_back(testY2);
	}

	void draw()
	{
		glColor3i(color.r, color.g, color.b);
		glBegin(GL_POINTS);
		int startX, startY, endX, endY;
		if (x1 <= x2) {
			startX = x1;
			startY = y1;
			endX = x2;
			endY = y2;
		}
		else {
			startX = x2;
			startY = y2;
			endX = x1;
			endY = y1;
		}
		x1 = startX;
		y1 = startY;
		x2 = endX;
		y2 = endY;
		vector<Point> pointsList;
		if (option == 0) {
			cout << "DDA algorithm\n";
			pointsList = drawLineDDA();
		}
		else {
			cout << "Bresenham algorithm\n";
			pointsList = drawLineBresenham();
		}
		glEnd();
	}

	void updateVertices(vector<Point> &vertices)
	{
	}
};

class Circle : public Shape {
private:
	int xt, yt, r;

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
	Circle(RGBColor &other)
	{
		this->color = other;
	};
	~Circle(){};

	void readInput(ifstream &fin)
	{
		fin >> xt >> yt >> r;
	}

	void draw()
	{
		int x = 0,
			y = r,
			p = 1 - r;

		while (x <= y) {
			drawCorrespondingPoints(x, y);
			++x;
			if (p < 0) {
				p = p + 2 * x + 1;
			}
			else {
				--y;
				p = p + 2 * x - 2 * y + 1;
			}
		}
	}

	// draw the OpenGL implementation
	void drawOpenGL(vector<int> &results)
	{
		verticalOffset = yt + r + 100;
		int newXt = xt, newYt = yt + verticalOffset;
		int testX1 = newXt - r, testY1 = newYt - r, testX2 = newXt + r, testY2 = newYt + r;

		glBegin(GL_LINE_LOOP);
		int numSegments = 100;
		for (int angle = 0; angle < 360; angle += 360 / numSegments) {
			float theta = angle * 3.14159 / 180,
				  x = r * cosf(theta),
				  y = r * sinf(theta);
			glVertex2f(newXt + x, newYt + y);
		}
		glEnd();
	}

	void updateVertices(vector<Point> &vertices)
	{
		Point startPoint = vertices[0],
			  endPoint = vertices[1];
		int width = abs(startPoint.getX() - endPoint.getX()),
			height = abs(startPoint.getY() - endPoint.getY()),
			sideLength = min(width, height),
			minX = min(startPoint.getX(), endPoint.getX()),
			minY = min(startPoint.getY(), endPoint.getY());
		r = sideLength / 2;
		xt = minX + r;
		yt = minY + r;
	}

	bool inside(int x, int y)
	{
		return (xt - x) * (xt - x) + (yt - y) * (yt - y) <= r * r;
	}

	double distanceToCenter(int x, int y)
	{
		return sqrt((xt - x) * (xt - x) + (yt - y) * (yt - y));
	}

	void scanLineColoring(RGBColor fillingColor)
	{
		int numSegments = 100;
		for (int angle = 0; angle < 180; angle += 360 / numSegments) {
			float theta = angle * 3.14159 / 180,
				  x = r * cosf(theta),
				  y = r * sinf(theta);
			int xR = xt + x,
				yR = yt + y,
				xL = xt - x,
				yL = yt - y;
			Line *line = new Line(xL, yL, xR, yR);
			line->draw();
			delete line;
		}
	}
};

class Ellipse : public Shape {
private:
	int xt, yt, a, b;

public:
	Ellipse() {}
	Ellipse(int xt, int yt, int a, int b)
		: xt(xt), yt(yt), a(a), b(b) {}
	Ellipse(RGBColor other)
	{
		this->color = other;
	};

	void readInput(ifstream &fin)
	{
		fin >> xt >> yt >> a >> b;
	}

	void drawCorrespondingPoints(int x, int y)
	{
		glBegin(GL_POINTS);
		plot(xt + x, yt + y);
		plot(xt + x, yt - y);
		plot(xt - x, yt + y);
		plot(xt - x, yt - y);
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

	void drawOpenGL(vector<int> &results)
	{
		verticalOffset = yt + b + 100;
		int newXt = xt, newYt = yt + verticalOffset;
		int testX1 = newXt - a, testY1 = newYt - b, testX2 = newXt + a, testY2 = newYt + b;

		glBegin(GL_LINE_LOOP);
		int numSegments = 100;
		for (int angle = 0; angle < 360; angle += 360 / numSegments) {
			float theta = angle * 3.14159 / 180,
				  x = a * cosf(theta),
				  y = b * sinf(theta);
			glVertex2f(newXt + x, newYt + y);
		}
		glEnd();
	};

	void updateVertices(vector<Point> &vertices)
	{
		Point startPoint = vertices[0],
			  endPoint = vertices[1];
		int horizontalSideLength = abs(startPoint.getX() - endPoint.getX()),
			verticalSideLength = abs(startPoint.getY() - endPoint.getY()),
			minX = min(startPoint.getX(), endPoint.getX()),
			minY = min(startPoint.getY(), endPoint.getY());
		a = horizontalSideLength / 2;
		b = verticalSideLength / 2;
		xt = minX + a;
		yt = minY + b;
		// draw();
	}

	bool inside(int x, int y)
	{
		return square(b) * square(x - xt) + square(a) * square(y - yt) <= square(a) * square(b);
	}

	double distance(int x, int y)
	{
		return sqrt(square(x - xt) + square(y - yt));
	}

	void scanLineColoring(RGBColor fillingColor)
	{
		int numSegments = 100;
		for (int angle = 0; angle < 360; angle += 360 / numSegments) {
			float theta = angle * 3.14159 / 180,
				  x = a * cosf(theta),
				  y = b * sinf(theta);
			int xR = xt + x,
				yR = yt + y,
				xL = xt - x,
				yL = yt - y;
			Line *line = new Line(xL, yL, xR, yR);
			line->draw();
			delete line;
		}
	}
};

class Rectangle : public Shape {
private:
	int x1, y1, x2, y2;

public:
	Rectangle() {}

	void readInput(ifstream &fin)
	{
		fin >> x1 >> y1 >> x2 >> y2;
	}

	void draw()
	{
		Line line(x1, y1, x1, y2, color);
		line.draw();
		line = Line(x1, y1, x2, y1, color);
		line.draw();
		line = Line(x2, y2, x2, y1, color);
		line.draw();
		line = Line(x2, y2, x1, y2, color);
		line.draw();
	}

	void drawOpenGL(vector<int> &results){};

	void updateVertices(vector<Point> &vertices)
	{
		Point startPoint = vertices[0],
			  endPoint = vertices[1];
		x1 = min(startPoint.getX(), endPoint.getX());
		y1 = min(startPoint.getY(), endPoint.getY());
		x2 = max(startPoint.getX(), endPoint.getX());
		y2 = max(startPoint.getY(), endPoint.getY());
	}

	bool inside(int x, int y)
	{
		return x1 < x && x < x2 && y1 < y && y < y2;
	}

	double distance(int x, int y)
	{
		int centerX = (x1 + x2) / 2,
			centerY = (y1 + y2) / 2;
		return sqrt(square(centerX - x) + square(centerY - y));
	}

	void scanLineAlgorithm(RGBColor color)
	{
		int y = y1;
		while (y < y2) {
			Line *line = new Line(x1, y, x2, y, color);
			line->draw();
			delete line;
			++y;
		}
	}
};

class Polygon : public Shape {
private:
	double xt, yt; // mean of the points of the polygon
	int rotateDegree = 10;
	int minY = HEIGHT + 5, maxY = -1, // start and end for scan line algorithm
		edgeCount = 0;				  // number of edges
	vector<Point> vertices;			  // list of vertices
	vector<double> slopeInv;		  // list of slope inverse of each edge
	vector<int> xMin, yMax;			  // list of x-coordinate of point with minimum y-coordinate, y-coordinate of point with maximum y-coordinate for each edge
	vector<vector<int>> edgeTable;	  // table storing the edges at its starting line entry

	int normalize(int y)
	{
		return y - minY;
	}

	void updateEdge(int x1, int y1, int x2, int y2)
	{
		double mInv = 1.0 * (x2 - x1) * (y2 - y1);
		if (y1 > y2) {
			int tempX = x2, tempY = y2;
			x2 = x1;
			y2 = y1;
			x1 = tempX;
			y1 = tempY;
		}
		xMin.push_back(x1);
		yMax.push_back(y2);
		slopeInv.push_back(mInv);
		edgeTable[normalize(y1)].push_back(edgeCount++);
	}

public:
	Polygon(){};

	void readInput(ifstream &fin)
	{
	}

	void draw()
	{
		edgeTable.assign(maxY - minY, vector<int>());
		int n = vertices.size();

		for (int i = 1; i < n; ++i) {
			int x1 = vertices[i - 1].getX(), y1 = vertices[i - 1].getY(), x2 = vertices[i].getX(), y2 = vertices[i].getY();
			Line *line = new Line(x1, y1, x2, y2, color);
			line->draw();
			delete line;
			updateEdge(x1, y1, x2, y2);
		}

		if (n > 2) {
			int x1 = vertices[n - 1].getX(), y1 = vertices[n - 1].getY(), x2 = vertices[0].getX(), y2 = vertices[0].getY();
			Line *line = new Line(x1, y1, x2, y2, color);
			line->draw();
			delete line;
			updateEdge(x1, y1, x2, y2);
		}
	}

	// draw the OpenGL implementation
	void drawOpenGL(vector<int> &results)
	{
	}

	void updateVertices(vector<Point> &vertices)
	{
		this->vertices = vertices;
		for (Point vertex : vertices) {
			minY = min(minY, vertex.getY());
			maxY = max(maxY, vertex.getY());
		}
		for (Point vertex : vertices) {
			xt += vertex.getX();
			yt += vertex.getY();
		}
		xt /= double(vertices.size());
		yt /= double(vertices.size());
	}

	bool inside(int x, int y)
	{
		return true;
	}

	double distanceToCenter(int x, int y)
	{
		return sqrt(square(x - xt) + square(y - yt));
	}

	void insert(vector<double> &xIntercept, list<int> &lst, int n, int value)
	{
		list<int>::iterator it = lst.begin();
		while (xIntercept[*it] < value) {
			++it;
		}
		lst.insert(it, n);
	}

	void scanLineColoring(RGBColor fillingColor)
	{
		list<int> activeList;
		vector<double> xIntercept(edgeCount);
		for (int i = 0; i < edgeCount; ++i) {
			xIntercept[i] = double(xMin[i]);
		}
		for (int scanY = minY; scanY < maxY; ++scanY) {
			// add new edge to active edges list
			for (int edge : edgeTable[normalize(scanY)]) {
				/* insert the new edge in a way that keeps the active list sorted 
				in an ascending order of the xIntercept value*/
				insert(xIntercept, activeList, edge, xMin[edge]);
			}

			auto cur = activeList.begin(), prev = activeList.begin();
			++cur;
			// parity bit
			int parity = 0;
			// iterate through each pair of adjacent points
			for (; cur != activeList.end(); ++cur, ++prev) {
				if (xIntercept[*cur] == xIntercept[*prev]) {
					if (slopeInv[*cur] * slopeInv[*prev] < 0) {
						/* if the two edges have the same intercept (this is their intersection) 
						and their slope have different signs */
						continue;
					}
				}
				// update the parity bit
				parity = (parity + 1) % 2;
				// if the parity bit is even
				if (parity == 0) {
					// draw
					for (int x = xIntercept[*prev]; x != xIntercept[*cur]; ++x) {
						plot(x, scanY, fillingColor);
					}
				}
			}

			// erase edges which is finished with its drawing
			cur = activeList.begin();
			while (cur != activeList.end()) {
				if (yMax[*cur] == scanY) {
					cur = activeList.erase(cur);
				}
				else {
					++cur;
				}
			}

			// update the x-intercepts of the points
			for (cur = activeList.begin(); cur != activeList.end(); ++cur) {
				xIntercept[*cur] += slopeInv[*cur];
			}
		}
	}

	void unSelect()
	{
		color = black;
	};

	void switchToSelected(RGBColor focusColor)
	{
		color = focusColor;
	};

	void transform(int option)
	{
		switch (option) {
		case ROTATE_LEFT:
			rotate(ROTATE_LEFT);
			break;
		case ROTATE_RIGHT:
			rotate(ROTATE_RIGHT);
			break;
		case ENLARGE:
			break;
		case SHRINK:
			break;
		case MOVE_LEFT:
			break;
		case MOVE_RIGHT:
			break;
		case MOVE_UP:
			break;
		case MOVE_DOWN:
			break;
		}
	}

	void rotate(int option)
	{
		printf("Rotate counterclockwise\n");
		int n = vertices.size();
		double degree;
		if (option == ROTATE_LEFT)
			degree = -rotateDegree;
		else
			degree = rotateDegree;
		for (int i = 0; i < n; ++i) {
			double radian = degree * PI / 180.0;
			Point vertex = vertices[i];
			int newX = xt + (vertex.getX() - xt) * cos(radian) - (vertex.getY() - yt) * sin(radian),
				newY = yt + (vertex.getX() - xt) * sin(radian) + (vertex.getY() - yt) * cos(radian);
			vertices[i].setX(newX);
			vertices[i].setY(newY);
		}
	}
};