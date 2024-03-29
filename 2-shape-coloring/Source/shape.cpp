
#include "shape.h"
#include <stack>

enum MENU_OPTION {
	DRAW_CIRCLE,
	DRAW_ELLIPSE,
	DRAW_RECTANGLE,
	DRAW_POLYGON
};

enum COLOR_MENU_OPTION {
	RED,
	GREEN,
	BLUE
};

enum PROGRAM_STATE {
	GLOBAL,	  // Nothing is selected
	DRAWING,  // In the middle of drawing an image
	FF_COLORING, // During Flood Fill coloring
	SL_COLORING, // During Scan Line coloring
};

static vector<Shape *> shapes;

class MenuHandler {
private:
	static int menu,
		remainingClicks,
		programState;
	static RGBColor fillingColor;
	static Shape *shape;
	static vector<Point> vertices;

	static int distanceToShape(Shape* shape, int x, int y) {
		if (!shape->inside(x, y)) {
			return -1;
		}

		return shape->distanceToCenter(x, y);
	}

	static void floodFillColoring(int x, int y)
	{
		RGBColor borderColor(0, 0, 0);
		stack<pair<int, int>> S;
		S.push({x, y});
		while (!S.empty()) {
			pair<int, int> top = S.top();
			S.pop();
			x = top.first, y = top.second;
			// cout << x << ' ' << y << endl;

			if (bitMap[x][y] == fillingColor || bitMap[x][y] == borderColor) {
				continue;
			}

			bitMap[x][y] = fillingColor;

			int dx[] = {1, 0, -1, 0};
			int dy[] = {0, 1, 0, -1};

			for (int i = 0; i < 4; ++i) {
				int newX = x + dx[i],
					newY = y + dy[i];
				if (1 <= newX && newX <= WIDTH && 1 <= newY && newY <= HEIGHT && bitMap[newX][newY] != fillingColor && bitMap[newX][newY] != borderColor) {
					S.push({newX, newY});
				}
			}
		}
	}

	static void scanLineColoring(int x, int y)
	{
		Shape* chosenShape = NULL;
		double minDist = 1e9, curDist;
		for (Shape* shape : shapes) {
			curDist = distanceToShape(shape, x, y);
			cout << curDist << endl;
			if (curDist != -1 && curDist < minDist) {
				minDist = curDist;
				chosenShape = shape;
			}
		}

		shape->scanLineColoring(fillingColor);
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
			printf("User chooses DRAW_RECTANGLE\n");
			remainingClicks = 2;
			shape = new Rectangle();
			programState = DRAWING;
			break;
		case DRAW_POLYGON:
			printf("User chooses DRAW_POLYGON\n");
			remainingClicks = 99999;
			shape = new Polygon();
			programState = DRAWING;
			break;
		default:
			break;
		}
		glutPostRedisplay();
		return;
	}

	static void floodFillColoringMenuEvents(int option)
	{
		switch (option) {
		case RED:
			printf("User colors with RED\n");
			programState = FF_COLORING;
			fillingColor = RGBColor(255, 0, 0);
			break;
		case GREEN:
			printf("User colors with GREEN\n");
			programState = FF_COLORING;
			fillingColor = RGBColor(0, 255, 0);
			break;
		case BLUE:
			printf("User colors with BLUE\n");
			programState = FF_COLORING;
			fillingColor = RGBColor(0, 0, 255);
			break;
		default:
			break;
		}
		glutPostRedisplay();
		return;
	}

	static void scanLineColoringMenuEvents(int option)
	{
		switch (option) {
		case RED:
			printf("User colors with RED\n");
			programState = SL_COLORING;
			fillingColor = RGBColor(255, 0, 0);
			break;
		case GREEN:
			printf("User colors with GREEN\n");
			programState = SL_COLORING;
			fillingColor = RGBColor(0, 255, 0);
			break;
		case BLUE:
			printf("User colors with BLUE\n");
			programState = SL_COLORING;
			fillingColor = RGBColor(0, 0, 255);
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

			if (programState == FF_COLORING && state == GLUT_DOWN) {
				floodFillColoring(x, y);
				glutPostRedisplay();
				return;
			}

			if (programState == SL_COLORING && state == GLUT_DOWN) {
				floodFillColoring(x, y);
				glutPostRedisplay();
				return;
			}

			if (programState == DRAWING && state == GLUT_DOWN) {
				vertices.push_back(Point(x, y));
				--remainingClicks;
				if (remainingClicks == 0) {
					glColor3ub(0, 0, 0);
					shape->updateVertices(vertices);
					shapes.push_back(shape);
					programState = GLOBAL;
					vertices.clear();
					glutPostRedisplay();
				}
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
		int floodFillColoringMenu = glutCreateMenu(floodFillColoringMenuEvents);
		glutAddMenuEntry("Red", RED);
		glutAddMenuEntry("Green", GREEN);
		glutAddMenuEntry("Blue", BLUE);

		int scanLineColoringMenu = glutCreateMenu(scanLineColoringMenuEvents);
		glutAddMenuEntry("Red", RED);
		glutAddMenuEntry("Green", GREEN);
		glutAddMenuEntry("Blue", BLUE);

		menu = glutCreateMenu(processMenuEvents);
		glutAddMenuEntry("Draw a circle", DRAW_CIRCLE);
		glutAddMenuEntry("Draw an ellipse", DRAW_ELLIPSE);
		glutAddMenuEntry("Draw a rectangle", DRAW_RECTANGLE);
		glutAddMenuEntry("Draw a polygon", DRAW_POLYGON);
		glutAddSubMenu("FF Coloring", floodFillColoringMenu);
		glutAddSubMenu("SL Coloring", scanLineColoringMenu);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
};

int MenuHandler::remainingClicks = -1,
	MenuHandler::programState = GLOBAL,
	MenuHandler::menu = 0;
Shape *MenuHandler::shape = NULL;
vector<Point> MenuHandler::vertices = vector<Point>(0);
RGBColor MenuHandler::fillingColor = RGBColor(0, 0, 0);

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(1.0);
	for (Shape *shape : shapes) {
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
	handler->createMenu();
	glutMainLoop();

	// free the dynamic memory
	for (Shape *shape : shapes) {
		delete shape;
	}
	return 0;
}