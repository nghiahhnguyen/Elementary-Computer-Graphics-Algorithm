
#include "shape.h"

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
		else if (geometricShape == "circle") {
			shape = new Circle();
		}
		else if (geometricShape == "ellipse") {
			shape = new Ellipse();
		}
		else if (geometricShape == "parabola") {
			shape = new Parabola();
		}
		else if (geometricShape == "hyperbola") {
			shape = new Hyperbola();
		}
		else if (geometricShape.substr(0, 4) == "skip") {
			// consume the line but do nothing
			shape = new Line();
			shape->convert(fin);
			continue;
		}
		else {
			printf("Invalid shape\n");
			fin.close();
			return;
		}

		enum Option {
			ORIGINAL,
			ADD_OPENGL_IMPLEMENTATION,
			ADD_OPENGL_IMPLEMENTATION_CALCULATION
		};

		shape->convert(fin);
		shape->readInput();

		auto start = chrono::high_resolution_clock::now();
		shape->draw();
		auto end = chrono::high_resolution_clock::now();
		cout << "The time to draw a/an " << geometricShape << " is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";

		if (shape->getOption() == ADD_OPENGL_IMPLEMENTATION) {
			vector<int> results;
			shape->drawOpenGLWithClock(results);
		}
		else if (shape->getOption() == ADD_OPENGL_IMPLEMENTATION_CALCULATION) {
			vector<Point> pointsList;
			shape->calculateDistance(pointsList);
		}
	}
	cout << "================\n\nEnd of a renderScence call.\n\n";
	fin.close();

	glFlush();
}

int main(int argc, char **argv)
{
	int width = 1200,
		height = 1080;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(width, height); // set the size of the window
	glutInitWindowPosition(10, 10);	// the position of the top-left of window
	glutCreateWindow("OpenGL Drawing shapes");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0 * width, 0.0, 1.0 * height);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutMainLoop();

	return 0;
}