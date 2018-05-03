#include "stdafx.h"
#include <string>
#include "freeglut_std.h"
#include <iostream>
#include <stdio.h>
#include "freeglut.h"
#include "gl.h"
#include "glext.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PI 3.14159265

using namespace std;

static int points = 0;
GLboolean isAnimating = true;
class Missile {
	const static GLint speed = 3;
private:
	GLdouble x_speed;
	GLdouble y_speed;
	GLdouble x_position;
	GLdouble y_position;
	GLdouble angle;

public:
	bool real = false;
	Missile() {
	}

	Missile(double x_posM, double y_posM, double x_speedM, double y_speedM) {
		//x_position = x_posM ;
		//y_position = y_posM ;

		x_position = 0;
		y_position = 0;
		x_speed = x_speedM * 6;
		y_speed = y_speedM * 6;
		real = true;
	}

	void drawMissile() {
		if (real)
		{
			glLoadIdentity();
			glColor3d(1.0, .4, .4);

			glBegin(GL_LINES);
			glVertex2d(x_position, y_position);
			glVertex2d(x_position + 1 * x_speed, y_position + 1 * y_speed);
			glEnd();

			x_position += x_speed;
			y_position += y_speed;
		}
	}
	bool collision(double x_posA, double y_posA, double size) {

		if ((x_posA + size > x_position && x_posA - size < x_position) && (y_posA + size > y_position && y_posA - size < y_position)) {
			real = false;
			points += 10;
			cout << "HIT ! Points = " << points;
			return true;

		}
		return false;
	}
};

class Astroid {
public:

	GLdouble x_pos;
	GLdouble y_pos;
	GLdouble size;
	GLdouble rotate = 0;
	bool destroyed = false;
	bool real = true;

	double steps = 2 * PI / 7;

	Astroid() {}
	Astroid(double x_posA, double y_posA, double size) {
		x_pos = x_posA;
		y_pos = y_posA;
		this->size = size;
		real = true;
	}

	bool collision(double x_posA, double y_posA, double sizeA) {

		if ((x_pos + sizeA + size > x_posA && x_pos - sizeA - size < x_posA) && (y_pos + sizeA + size > y_posA && y_pos - sizeA - size< y_posA)) {
			return true;

		}
		return false;
	}

	void drawAstroid() {
		glLoadIdentity();
		glTranslated(x_pos, y_pos, 0.0);
		glRotated(rotate, 0.0, 0.0, 1.0);
		rotate += 1;
		glColor3d(0.45, 0.4, 0.2);

		glBegin(GL_TRIANGLE_FAN);
		for (double t = 0; t <= 2 * PI; t += steps) {
			glVertex2d(size*cos(t), size * sin(t));
		}

		if (rotate > 360) {
			rotate = 0;
		}

		glEnd();
	}
};


// GLdouble speed_ship = 0.5 ; // Pre defined speed of the SpaceShip
GLint angle_ship = 0; // Rotation of the space ship

GLdouble x_pos = 0.0;
GLdouble y_pos = 0.0;

GLdouble x_speedM = 1 * cos(angle_ship*PI / 180);
GLdouble y_speedM = 1 * sin(angle_ship*PI / 180);

Missile fired[20];
int count1 = 0;

Astroid astroids[10];

bool collideWithOtherA(double x_posIN, double y_posIN, double sizeIN) {
	cout << "Checking -- ";
	for (int i = 0; i < 10; i++) {
		if (astroids[i].real) {
			bool colide = astroids[i].collision(x_posIN, y_posIN, sizeIN);
			if (colide) {
				return true;
			}
		}
	}
	return false;
}

static int astrods = 0;

void CreateAstroid(double x_posA, double y_posA, double size, int index) {
	cout << "\n Created -- " << index << " \n";
	Astroid as(x_posA, y_posA, size);
	astroids[index] = as;

}
void newAstroids() {
	cout << " Creating Astroids /n";
	for (int i = 0; i < 10; i++) {

		int sizeA = (rand() % 5 + 10);
		int x_posA = rand() % (200 - (2 * sizeA)) + (-100 + sizeA);
		int y_posA = rand() % (200 - (2 * sizeA)) + (-100 + sizeA);
		cout << x_posA << " " << y_posA << "\n";

		if ((x_posA + sizeA + 25 > 0 && x_posA - sizeA - 25 < 0) && (y_posA + sizeA + 25 > 0 && y_posA - sizeA - 25< 0)) {
			i--;

		}
		else
		{
			bool cont = collideWithOtherA(x_posA, y_posA, sizeA);
			if (!cont) {
				CreateAstroid(x_posA, y_posA, sizeA, i);
			}
			else
			{
				i--;
			}

		}


		astrods = 10;
	}
}

void drawAstroids() {
	if (astrods == 0) {
		newAstroids();
	}
	for (int i = 0; i < 10; i++) {
		if (astroids[i].real && !astroids[i].destroyed) {
			astroids[i].drawAstroid();
		}
	}
}

void bulletHit() {
	for (int j = 0; j < 10; j++) {
		if (astroids[j].real) {
			int x_posA = astroids[j].x_pos;
			int y_posA = astroids[j].y_pos;
			int size = astroids[j].size;
			bool hit = false;
			for (int i = 0; i < 20; i++) {
				if (fired[i].real) {
					hit = fired[i].collision(x_posA, y_posA, size);
					if (hit) {
						break;
					}
				}
			}
			if (hit) {
				astroids[j].destroyed = true;
				astroids[j].real = false;
			}
		}

	}
}

void calcSpeed() {// call this everytime the angle is changed
	x_speedM = 1 * cos(angle_ship*PI / 180);
	y_speedM = 1 * sin(angle_ship*PI / 180);
}
void reset() {
	x_pos = 0;
	y_pos = 0;
	//speed_ship = 0;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);

}

void shootMissile() {
	fired[count1].~Missile();
	Missile mis(x_pos, y_pos, x_speedM, y_speedM);
	fired[count1] = mis;
	if (count1 == 19) {
		count1 = 0;
	}
	else {
		count1++;
	}
}

void drawMissiles() {
	for (int i = 0; i <20; i++) {
		if (fired[i].real) {
			fired[i].drawMissile();
		}
	}
}

void spaceShip() {

	glLoadIdentity();
	glRotatef(angle_ship - 90, 0.0, 0.0, 1.0);

	glLineWidth(3); // Changing the line width property
	glColor3d(1.0, 0.4, 0.4); // Orangish color

	glBegin(GL_LINES);
	glVertex2d(-1.0, -2.5);
	glVertex2d(-1.0, -0.9);

	glVertex2d(1.0, -2.5);
	glVertex2d(1.0, -0.9);
	glEnd();
	glBegin(GL_TRIANGLES);

	glColor3d(0.0, 1.0, 0.0);// Green

	glVertex3f(0.0, 2.5, 0.0);
	glVertex3f(-2.5, -2.5, 0.0);
	glVertex3f(0.0, -1.5, 0.0);

	glColor3d(0.0, 0.88, 0.0); // Dark Green

	glVertex3f(0.0, 2.5, 0.0);
	glVertex3f(0.0, -1.5, 0.0);
	glVertex3f(2.5, -2.5, 0.0);

	glEnd();
}
void TimerFunc(int value)
{
	if (isAnimating)
	{
		glutPostRedisplay();
	}

	glutTimerFunc(25, TimerFunc, 1);
}

void special(int key, int, int)
{
	// handle special keys
	switch (key)
	{
		//case GLUT_KEY_UP: g_yoffset += g_offset_step; break;
	case GLUT_KEY_LEFT:
		angle_ship += 5;
		calcSpeed();
		break;
	case GLUT_KEY_RIGHT:
		angle_ship -= 5;
		calcSpeed();
		break;
	}
	// glutPostRedisplay(); // force a redraw
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case ' ': shootMissile(); break;
	}
	//glutPostRedisplay();
}

int oldPoints = 0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (points % 100 == 0 && points != oldPoints) {
		newAstroids();
		oldPoints = points;
	}

	drawMissiles();
	drawAstroids();
	bulletHit();
	spaceShip();
	glutSwapBuffers();

}

int main(int argc, char* argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 800);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Hello OpenGL");
	// Bind the two functions (above) to respond when necessary
	glutTimerFunc(25, TimerFunc, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(special);
	// glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}