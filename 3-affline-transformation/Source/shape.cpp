
#include "shape.h"
#include <stack>

enum MENU_OPTION {
	SELECT
};

enum COLOR_MENU_OPTION {
	RED,
	GREEN,
	BLUE
};

enum PROGRAM_STATE {
	GLOBAL, // Nothing is selected
	DRAWING,
	SELECTING,
	SELECTED
};

static vector<Shape *> shapes;

class MenuHandler {
private:
	static int menu,
		remainingClicks,
		programState;
	static RGBColor fillingColor, focusColor;
	static Shape *shape, *selectedShape;
	static vector<Point> vertices;

	static int distanceToShape(Shape *shape, int x, int y)
	{
		if (!shape->inside(x, y)) {
			return -1;
		}

		return shape->distanceToCenter(x, y);
	}

	static void selectShape(int x, int y)
	{
		selectedShape = NULL;
		double minDist = 1e9, curDist;
		for (Shape *shape : shapes) {
			curDist = distanceToShape(shape, x, y);
			cout << curDist << endl;
			if (curDist != -1 && curDist < minDist) {
				minDist = curDist;
				selectedShape = shape;
			}
		}

		selectedShape->switchToSelected(focusColor);
	}

	static void processMenuEvents(int option)
	{
		switch (option) {
		case SELECT:
			printf("User chooses SELECT\n");
			programState = SELECTING;
			break;
		default:
			break;
		}
		glutPostRedisplay();
		return;
	}

public:
	static void mouseActionHandler(int button, int state, int x, int y)
	{
		switch (button) {
		case GLUT_LEFT_BUTTON:
			printf("Left click detected\n");
			if (state == GLUT_UP)
				if (remainingClicks == 0)
					return;

			if (programState == GLOBAL && state == GLUT_DOWN)
				programState = DRAWING;
			if (programState == DRAWING && state == GLUT_DOWN) {
				vertices.push_back(Point(x, y));
				// --remainingClicks;
				// if (remainingClicks == 0) {
				// 	glColor3ub(0, 0, 0);
				// 	shape->updateVertices(vertices);
				// 	shapes.push_back(shape);
				// 	programState = GLOBAL;
				// 	vertices.clear();
				// 	glutPostRedisplay();
				// }
			}

			if (programState == SELECTED && state == GLUT_DOWN) {
				selectedShape->unSelect();
				programState = GLOBAL;
				glutPostRedisplay();
			}

			if (programState == SELECTING && state == GLUT_DOWN) {
				selectShape(x, y);
				programState = SELECTED;
				glutPostRedisplay();
			}
			break;
		case GLUT_RIGHT_BUTTON:

			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_UP)
				if (remainingClicks == 0)
					return;

			remainingClicks = 0;

			printf("Middle click detected\n");
			if (programState == DRAWING && state == GLUT_DOWN) {
				glColor3ub(0, 0, 0);
				shape->updateVertices(vertices);
				shapes.push_back(shape);
				vertices.clear();
				shape = new Polygon();
				programState = GLOBAL;
				glutPostRedisplay();
			}
			break;
		default:
			printf("Unknown mouse event.\n");
			break;
		}
	}

	static void createMenu()
	{
		menu = glutCreateMenu(processMenuEvents);
		glutAddMenuEntry("SELECT", SELECT);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

	static void processNormalKeys(unsigned char key, int x, int y)
	{
		if (programState == SELECTED) {
			switch (key) {
			case 43:
				printf("+ was pressed\n");
				selectedShape->transform(ENLARGE);
				break;
			case 45:
				printf("- was pressed\n");
				selectedShape->transform(SHRINK);
				break;
			case 108:
				printf("\'l\' was pressed\n");
				selectedShape->transform(ROTATE_LEFT);
				break;
			case 114:
				printf("\'r\' was pressed\n");
				selectedShape->transform(ROTATE_RIGHT);
				break;
			default:
				break;
			}
			glutPostRedisplay();
		}
	}

	static void processSpecialKeys(int key, int x, int y)
	{
		if (programState == SELECTED) {
			switch (key) {
			case GLUT_KEY_LEFT:
				printf("LEFT was pressed\n");
				selectedShape->transform(MOVE_LEFT);
				break;
			case GLUT_KEY_RIGHT:
				printf("RIGHT was pressed\n");
				selectedShape->transform(MOVE_RIGHT);
				break;
			case GLUT_KEY_UP:
				printf("UP was pressed\n");
				selectedShape->transform(MOVE_UP);
				break;
			case GLUT_KEY_DOWN:
				printf("DOWN was pressed\n");
				selectedShape->transform(MOVE_DOWN);
				break;
			default:
				break;
			}
			glutPostRedisplay();
		}
	}
};

int MenuHandler::remainingClicks = -1,
	MenuHandler::programState = GLOBAL,
	MenuHandler::menu = 0;
Shape *MenuHandler::shape = new Polygon(), *MenuHandler::selectedShape = NULL;
vector<Point> MenuHandler::vertices = vector<Point>(0);
RGBColor MenuHandler::fillingColor = RGBColor(0, 0, 0), MenuHandler::focusColor = RGBColor(255, 0, 0);

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	// clear the bitmap to erase old drawings
	for (int i = 1; i <= WIDTH; ++i) {
		for (int j = 1; j <= HEIGHT; ++j) {
			bitMap[i][j] = RGBColor(255, 255, 255);
		}
	}

	int shapeCount = 1;
	for (Shape *shape : shapes) {
		printf("Drawing shape %d\n", shapeCount++);
		shape->draw();
	}

	RGBColor tempColor;
	for (int i = 1; i <= WIDTH; ++i) {
		for (int j = 1; j <= HEIGHT; ++j) {
			tempColor = bitMap[i][j];
			int R = int(tempColor.r), G = int(tempColor.g), B = int(tempColor.b);
			glColor3ub(R, G, B);
			glBegin(GL_POINTS);
			glVertex2i(i, j);
			glEnd();
		}
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	int width = WIDTH,
		height = HEIGHT;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height); // set the size of the window
	glutInitWindowPosition(10, 10);	   // the position of the top-left of window
	glutCreateWindow("OpenGL Drawing shapes");
	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	// glPointSize(1.0);
	glFlush();
	glutSwapBuffers();

	// register callbacks
	glutDisplayFunc(renderScene); // create menu
	MenuHandler *handler = new MenuHandler();
	glutMouseFunc(handler->mouseActionHandler);
	glutKeyboardFunc(handler->processNormalKeys);
	glutSpecialFunc(handler->processSpecialKeys);
	handler->createMenu();

	glutMainLoop();

	// free the dynamic memory
	for (Shape *shape : shapes) {
		delete shape;
	}
	return 0;
}