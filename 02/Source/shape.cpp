
#include "shape.h"
#include <mutex>

enum MENU_OPTION {
	DRAW_CIRCLE,
	DRAW_ELLIPSE,
	DRAW_RECTANGLE,
	DRAW_POLYGON
};

enum PROGRAM_STATE {
	GLOBAL, // Nothing is selected
	DRAWING // In the middle of drawing an image
};

static vector<Shape *> shapes;

class MenuHandler {
private:
	static int menu,
		remainingClicks,
		programState;
	static Shape *shape;
	static vector<Point> vertices;

public:
	static void mouseActionHandler(int button, int state, int x, int y)
	{
		switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP)
				if (remainingClicks == 0)
					return;
			if (programState == DRAWING && state == GLUT_DOWN) {
				vertices.push_back(Point(x, y));
				--remainingClicks;
				if (remainingClicks == 0) {
					// glClear(GL_COLOR_BUFFER_BIT);
					// glPointSize(1.0);
					glColor3f(1.0, 0.0, 0.0);
					shape->drawFromVertices(vertices);
					// delete shape;
					shapes.push_back(shape);
					programState = GLOBAL;
					vertices.clear();
					glutPostRedisplay();
					// glFlush();
					// glutSwapBuffers();
					// glutDetachMenu(menu);
					// glutDestroyMenu(menu);
				}
			}
			break;
		case GLUT_RIGHT_BUTTON:
			// if (state == GLUT_DOWN) {
			// 	createMenu();
			// }
			// else {
			// }
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		default:
			printf("Unknown mouse event.\n");
			break;
		}
	}

	static void processMenuEvents(int option)
	{
		switch (option) {
		case DRAW_CIRCLE:
			printf("User chooses DRAW_CIRCLE\n");
			remainingClicks = 2;
			shape = new Circle();
			programState = DRAWING;
			break;
		case DRAW_ELLIPSE:
			printf("User chooses DRAW_ELLIPSE\n");
			remainingClicks = 2;
			shape = new Ellipse();
			programState = DRAWING;
			break;
		case DRAW_RECTANGLE:
		case DRAW_POLYGON:
		default:
			break;
		}
		glutPostRedisplay();
		return;
	}

	static void createMenu()
	{
		menu = glutCreateMenu(processMenuEvents);

		glutAddMenuEntry("Draw a circle", DRAW_CIRCLE);
		glutAddMenuEntry("Draw an ellipse", DRAW_ELLIPSE);
		glutAddMenuEntry("Draw a rectangle", DRAW_RECTANGLE);
		glutAddMenuEntry("Draw a polygon", DRAW_POLYGON);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
		// glutDetachMenu(GLUT_LEFT_BUTTON);
	}
};

int MenuHandler::remainingClicks = -1,
	MenuHandler::programState = GLOBAL,
	MenuHandler::menu = 0;
Shape *MenuHandler::shape = NULL;
vector<Point> MenuHandler::vertices = vector<Point>(0);

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	for (Shape *shape : shapes) {
		shape->draw();
	}
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	int width = 1200,
		height = 1080;
	// TODO: make this singleton
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height); // set the size of the window
	glutInitWindowPosition(10, 10);	   // the position of the top-left of window
	glutCreateWindow("OpenGL Drawing shapes");
	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0 * width, 1.0 * height, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	// glPointSize(1.0);
	glFlush();
	glutSwapBuffers();
	// // register callbacks
	glutDisplayFunc(renderScene); // create menu
	MenuHandler *handler = new MenuHandler();
	glutMouseFunc(handler->mouseActionHandler);
	handler->createMenu();
	glutMainLoop();

	return 0;
}