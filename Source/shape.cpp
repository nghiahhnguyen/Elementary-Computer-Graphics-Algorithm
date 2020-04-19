
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