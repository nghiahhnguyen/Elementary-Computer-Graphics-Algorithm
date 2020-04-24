
#include "shape.h"

void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);

	ifstream fin;
	fin.open("INP.txt");
	// freopen("INP.txt", "r", stdin);
	int object;
	string geometricShape;
	Shape *shape = NULL;
	while (fin >> object) {
		glColor3f(1.0, 0.0, 0.0);

		if (shape != NULL)
			delete shape;

		if (object == 0) {
			shape = new Line(0);
			geometricShape = "line";
		}
		else if (object == 1) {
			shape = new Line(1);
			geometricShape = "line";
		}
		else if (object == 2) {
			shape = new Circle();
			geometricShape = "circle";
		}
		else if (object == 3) {
			shape = new Ellipse();
			geometricShape = "ellipse";
		}
		else if (object == 4) {
			shape = new Parabola();
			geometricShape = "parabola";
		}
		else if (object == 5) {
			shape = new Hyperbola();
			geometricShape = "hyperbola";
		}
		else {
			printf("Invalid shape\n");
			fin.close();
			return;
		}

		shape->readInput(fin);

		auto start = chrono::high_resolution_clock::now();
		shape->draw();
		auto end = chrono::high_resolution_clock::now();
		cout << "\nThe time to draw a/an " << geometricShape << " is " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " ms\n";

		// if (object <= 2) {
		// 	vector<int> results;
		// 	shape->drawOpenGLWithClock(results);
		// }
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