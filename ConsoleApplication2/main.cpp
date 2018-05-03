#include "stdafx.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include "freeglut.h"
#include "glext.h"
#include <math.h>

#define PI 3.14159265
#define MAX_ASTEROIDS 25
#define MIN_ASTEROIDS 4
#define MAX_BULLETS 25
#define NUM_OF_LEVELS 10
#define WIDTH 800
#define HEIGHT 600
#define SHIPSPEED 5

using namespace std;

static int level = 0;
static int score = 0;
static int levelAsteroids = MIN_ASTEROIDS;
GLboolean isAnimating = true;
GLboolean initial = true;
GLboolean change = false; // If Screen Width < 300 ? true : false
GLint nOfAsteroids = 0;

//If Grid or Polygon to be displayed
GLboolean grid = true;

//Stippled Polygon
GLubyte fly[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1f, 0x80, 0x1f, 0xc0, 0x0f, 0xc0, 0x3f, 0x80,
	0x03, 0xf5, 0xff, 0x00, 0x07, 0xfd, 0xff, 0xf8,
	0x1f, 0xfc, 0xff, 0x08, 0xff, 0x03, 0xbf, 0x70,
	0x03, 0x10, 0x40, 0x40, 0x08, 0x88, 0x8c, 0x20,
	0x05, 0x05, 0x04, 0x40, 0x02, 0x82, 0x14, 0x40,
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
};

// GLdouble speed_ship = 0.5 ; // Pre defined speed of the SpaceShip
GLint angle = 0; // Rotation of the space ship

int bulletsCount = 0;

//==================================Classes==================================//

//-------------------Asteroids Class--------------------//
class Asteroid {
public:

	GLdouble centerX, centerY, radius;
	GLint red, green, blue;
	GLint numOfSides;// Random Number of Sides for the Asteroid [Polygon]

	Asteroid() {}
	Asteroid(double radius, double centerX, double centerY) {
		this->radius = radius; // Maximum Length from the center of the Asteroid
		this->centerX = centerX; // Center X Coordinate
		this->centerY = centerY; // Center Y Coordinate
		this->red = rand() % 255 + 0; // Random Value for RED 
		this->green = rand() % 255 + 0; // Random Value for GREEN
		this->blue = rand() % 255 + 0; // Random Value for BLUE
		this->numOfSides = rand() % 3 + 5;

	}

};


//-------------------Bullets Class--------------------//
class Bullet {
public:
	GLdouble movementX, movementY, x, y;

	Bullet() {
	}

	Bullet(double angle) {
		this->x = 0; // Bullets are created at the origin - Position changes with movementX value
		this->y = 0; // Bullets are created at the origin - Position changes with movementY value
		this->movementX = cos(angle*PI/180);
		this->movementY = sin(angle*PI / 180);
	}

};
//==================================End Of Classes==================================//


Bullet *bullets[MAX_BULLETS]; // Store Bullets 
Asteroid *asteroids[MAX_ASTEROIDS]; // Store Asteroids

// Shoot the Bullets
void shoot();
// Draw the Score on the top left of the screen - Updated everytime the user shoots an Asteroid
void drawScore(void *font);
// Draw the Spaceship
void drawShip();
// Draw the Asteroids to be displayed
void drawAsteroid(int i);
// Draw the Bullets required by the spaceship to shoot the Asteroids
void drawBullet(int i);
// Draw the Player Level
void drawLevel(void *font);
// Game Over
void drawGameOver();

// Draw the Asteroids to be displayed - Polygons with number of sides (in the range 5 to 7)
void drawAsteroid(int i) {

	glLoadIdentity();
	glTranslated(asteroids[i]->centerX, asteroids[i]->centerY, 0.0); // Translate Asteroid to it's center (centerX,centerY,0.0) - 0.0 for z-axis
	glColor3ub(asteroids[i]->red, asteroids[i]->green, asteroids[i]->blue); // Color of the Asteroid - Red, Green and Blue

	glBegin(GL_POLYGON);
	for (int j = 0; j < asteroids[i]->numOfSides; j++) {
		glVertex2d(asteroids[i]->radius*cos(j), asteroids[i]->radius*sin(j)); // (x,y)
	}
	glEnd();
}

// Draw The Bullet to be shot by the player 
void drawBullet(int i) {

	glLoadIdentity();
	glColor3ub(100, 255, 140);

	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2d(bullets[i]->x, bullets[i]->y); // (x,y) to indicate the beginning point of the buller
	glVertex2d(bullets[i]->x + bullets[i]->movementX, bullets[i]->y + bullets[i]->movementY); // eg:- (x+1,y+1) to indicate the ending point bullet
	glEnd();

	// Incremented to keep the bullet moving
	bullets[i]->x += bullets[i]->movementX;
	bullets[i]->y += bullets[i]->movementY;
}

// Every 10 ms display() reposted
void TimerFunc(int value)
{
	if (isAnimating)
	{
		glutPostRedisplay();
	}

	glutTimerFunc(10, TimerFunc, 1);
}

// Special Key Input Check - Left : Spaceship turns to left | Right : Spaceship turns to right
void special(int key, int, int)
{
	// handle special keys
	switch (key)
	{
	case GLUT_KEY_LEFT: {
		angle += SHIPSPEED;
		break;
	}
	case GLUT_KEY_RIGHT: {
		angle -= SHIPSPEED;
		break;
	}

	}
}

//Key Input - Spacebar : Shoot Bullet
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ': {
		shoot();
		break;
	}
	case 27: {
		exit(0);
		break;
	}
	}
}

// Display Grid or Stippled Polygon Background - 1 : Grid | 2 : Stippled Polygon
void pattern(int item)
{
	switch (item)
	{
	case 1:
	{
		grid = true;
		break;
	}
	case 2: {
		grid = false;
	}
	default:
	{
		//Do Nothing */
		break;
	}
	}

	glutPostRedisplay();

	return;
}

/*
 * Check if the Bullet has collided with any of the Asteroids
 * If Bullet collided with the Asteroid, delete Asteroid and set bullet index to null pointer
 */
void detectCollision() {
	for (int j = 0; j < levelAsteroids; j++) {
		// If asteroid index is not null
		if (asteroids[j] != nullptr) {
			for (int i = 0; i < MAX_BULLETS; i++) {
				// If bullet index is not null
				if (bullets[i] != nullptr) {
					// If Bullet Collides with Asteroid, delete asteroid object and point bullets index to null pointer
					// Decrease Number of Asteroids
					// If Asteroid has collided with the bullet
					if ((asteroids[j]->centerX + asteroids[j]->radius > bullets[i]->x && asteroids[j]->centerX - asteroids[j]->radius < bullets[i]->x) && (asteroids[j]->centerY + asteroids[j]->radius > bullets[i]->y && asteroids[j]->centerY - asteroids[j]->radius < bullets[i]->y)) {
						delete asteroids[j];
						bullets[i] = nullptr;
						nOfAsteroids--;
						break;
					}
				}
			}
		}

	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);

	glMatrixMode(GL_MODELVIEW);

	if (w < 600) {
		change = true;
	}
	else {
		change = false;
	}

}

/*
* Shoot Bullets
* Create New Bullet Object directed at the angle the Spaceship is directed in and store it in bullets[]
* If BulletCount Limit Reached, Reset BulletsCount variable to 0
*/
void shoot() {
	bullets[bulletsCount] = new Bullet(angle);;
	if (bulletsCount == MAX_BULLETS-1) {
		bulletsCount = 0;
	}
	else {
		bulletsCount++;
	}
}

/*
 * Draw the Spaceship the player uses to shoot asteroids
 * Spaceship drawn at the center of the screen with a gradient color
 */
void drawShip() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRotatef(angle, 0.0, 0.0, 1.0);
	
	glBegin(GL_POLYGON);
	glColor3ub(0, 255, 25);
	glVertex2f(0.0, 0.0);
	glColor3ub(50, 150, 75);
	glVertex2f(-2.5, -2.5);
	glColor3ub(50, 255, 100);
	glVertex2f(2.5, 0.0);
	glVertex2f(-2.5, 2.5);
	glEnd();

	glPopMatrix();
}

/*
 * Generate a Random Radius, CenterX and CenterY for the Asteroids
 * Make Sure the Asteroid doesn't overlap with the Spaceship
*/
void randomizeAsteroid()
{
	int i = 0;

	while (i < levelAsteroids) {
		int radius = (rand() % 3 + 6); // Radius Min: 6 Max: 9
		int centerX = rand() % (100 - (2 * radius)) + (-50 + radius); // Generate Center - X Coordinate
		int centerY = rand() % (100 - (2 * radius)) + (-50 + radius); // Generate Center - Y Coordinate
		bool collides = false;

		// Check if Asteroid the values of the to be Generated Asteroid Collide with any existing Asteroids
		for (int i = 0; i < levelAsteroids; i++) {
			if ((centerX + radius + 5 > 0 && centerX - radius - 5 < 0) && (centerY + radius + 5 > 0 && centerY - radius - 5 < 0)) {
				collides = true;
				i--;
				break;
			}
			if (asteroids[i] != nullptr) {
				if ((asteroids[i]->centerX + asteroids[i]->radius > centerX && asteroids[i]->centerX - asteroids[i]->radius < centerX) && (asteroids[i]->centerY + asteroids[i]->radius > centerY && asteroids[i]->centerY - asteroids[i]->radius < centerY)) {
					collides = true;
					break;
				}
			}
		}
		

		// Check if Asteroid collides with another asteroid
		// If Asteroid Values collide with Other Asteroids, regenerate values - Go back to the previous index to create the Asteroid
		// If Asteroid Values don't collide with other Asteroids, continue generating the Actual Asteroid with the values
		if (collides) {
			i--;
		}
		else {
			Asteroid* obj = new Asteroid(radius, centerX, centerY);
			asteroids[i] = obj;
			nOfAsteroids++;
		}

		i++; // Increment i Counter
	}
	
}

/*
 * Draw the Current Score of the user in the Top left of the Screen
 */
void drawScore(void *font)
{
	string scoreString = "Score : " + to_string(score);
	glColor3ub(0, 150, 255); // Color of the Score Text
	glRasterPos2f(-47, 45); // Position of Score
	for (int i = 0; i < scoreString.length(); i++) {
		glutBitmapCharacter(font, scoreString[i]);
	}

}

/*
 * Draw the Current Level the user is in the Top Right of the Screen
 */
void drawLevel(void *font)
{
	string levelString = "Level : " + to_string(level);
	glColor3ub(0, 150, 255); // Color of the Score Text
	glRasterPos2f(35, 45); // Position of Score
	for (int i = 0; i < levelString.length(); i++) {
		glutBitmapCharacter(font, levelString[i]);
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display Grid or Stippled Polygon
	if (grid == true) {
		//Stippled Grid
		for (float x = -50; x < 50; x += 10) {
			glColor3ub(100, 100, 100);
			glLineWidth(1.0);
			glBegin(GL_LINES);
			glVertex2f(x, 50);
			glVertex2f(x, -50);
			glEnd();
		};

		for (float y = -50; y < 50; y += 10) {
			glColor3ub(100, 100, 100);
			glLineWidth(1.0);
			glBegin(GL_LINES);
			glVertex2f(50, y);
			glVertex2f(-50, y);
			glEnd();
		};
	}
	else {
		//Stippled Polygons
		glColor3ub(250, 255, 40);
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(fly);
		glRectf(-50, -50, 50, 50);
		glDisable(GL_POLYGON_STIPPLE);
	}

	if (level <= NUM_OF_LEVELS) {
		//Draw the Bullets
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i] != nullptr) {
				drawBullet(i);
			}
		}

		//Draw the Asteroids
		if (nOfAsteroids == 0) {
			randomizeAsteroid();
			level++;
			levelAsteroids++;
		}
		// Draw 'levelAsteroids' Number of Asteroids
		for (int i = 0; i < levelAsteroids-1; i++) {
			drawAsteroid(i);
		}
		//Check for Bullet Collision - Collision Detection
		detectCollision();
		//Draw the SpaceShip
		drawShip();
		if (change) {
			//Display the Score
			drawScore(GLUT_BITMAP_HELVETICA_12);
			//Display the Level
			drawLevel(GLUT_BITMAP_HELVETICA_12);
		}
		else {
			//Display the Score
			drawScore(GLUT_BITMAP_9_BY_15);
			//Display the Level
			drawLevel(GLUT_BITMAP_9_BY_15);
		}
	}
	else {
		isAnimating = false;
		drawGameOver();
	}

	glutSwapBuffers();

}

int main(int argc, char* argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Multi Level Asteroid Game");

	// Create a menu
	glutCreateMenu(pattern);

	// Add menu items
	glutAddMenuEntry("Grid On", 1);
	glutAddMenuEntry("Pattern On", 2);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Bind the two functions (above) to respond when necessary
	
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(special); // Keyboard Function
	glutKeyboardFunc(keyInput); // Keyboard Function
	glutTimerFunc(25, TimerFunc, 1); // Timer Function
	glutMainLoop();
	return 0;
}

// Display Game Over if all levels are completed
void drawGameOver()
{
	// Game Over Text
	string gameOverStr = "Game Over - All Levels Completed";
	glColor3ub(0, 231, 136);
	glRasterPos2f(-25, 10.0);
	for (int i = 0; i < gameOverStr.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverStr[i]);
	}

	// Score Text
	string scoreStr = "Your Score is: " + to_string(score);
	glColor3ub(200, 50, 50);
	glRasterPos2f(-25, -10.0);
	for (int i = 0; i < scoreStr.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreStr[i]);
	}

	// Exit Text
	string quitStr = "Press Esc to quit";
	glColor3ub(55, 250, 100);
	glRasterPos2f(-25, -25.0);
	for (int i = 0; i < quitStr.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, quitStr[i]);
	}
}