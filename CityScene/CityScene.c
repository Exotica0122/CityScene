/******************************************************************************
 *
 * Computer Graphics Programming 2020 Project Template v1.0 (11/04/2021)
 *
 * Based on: Animation Controller v1.0 (11/04/2021)
 *
 * This template provides a basic FPS-limited render loop for an animated scene,
 * plus keyboard handling for smooth game-like control of an object such as a
 * character or vehicle.
 *
 * A simple static lighting setup is provided via initLights(), which is not
 * included in the animationalcontrol.c template. There are no other changes.
 *
 * Author: Peter An
 *
 ******************************************************************************/

#include "CityScene.h"

 /******************************************************************************
 * Animation & Timing Setup
 ******************************************************************************/

 // Target frame rate (number of Frames Per Second).
#define TARGET_FPS 60				

// Ideal time each frame should be displayed for (in milliseconds).
const unsigned int FRAME_TIME = 1000 / TARGET_FPS;

// Frame time in fractional seconds.
// Note: This is calculated to accurately reflect the truncated integer value of
// FRAME_TIME, which is used for timing, rather than the more accurate fractional
// value we'd get if we simply calculated "FRAME_TIME_SEC = 1.0f / TARGET_FPS".
const float FRAME_TIME_SEC = (1000 / TARGET_FPS) / 1000.0f;

// Time we started preparing the current frame (in milliseconds since GLUT was initialized).
unsigned int frameStartTime = 0;

int frameCount = 0;

/******************************************************************************
 * GLUT Callback Prototypes
 ******************************************************************************/

void display(void);
void reshape(int width, int h);
void idle(void);

/******************************************************************************
 * Animation-Specific Function Prototypes (add your own here)
 ******************************************************************************/

void main(int argc, char **argv);
void init(void);
void think(void);
void initLights(void);
void initCameraPosition(void);
void initSkyPosition(void);
void initDroneCenterPosition(void);

void drawOrigin(void);

/******************************************************************************
 * Animation-Specific Setup (Add your own definitions, constants, and globals here)
 ******************************************************************************/

 // Render objects as filled polygons (1) or wireframes (0). Default filled.
int renderFillEnabled = 1;

// window dimensions
GLint windowWidth = 1280;
GLint windowHeight = 720;

//spot light
GLfloat spotlightPos[] = { 0.0f, 0.0f, 6.0f, 1.0f }; //remember fourth parameter is w (when w = 1.0 its a directional light)
GLfloat spotDirection[] = { 0.0f, 0.0f, 0.0f };
double angle = 0.0f;

/******************************************************************************
 * Entry Point (don't put anything except the main function here)
 ******************************************************************************/

void main(int argc, char **argv)
{
	// seed random
	srand(time(0));

	// Initialize the OpenGL window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("City Scene");

	// Set up the scene.
	init();

	// Disable key repeat (keyPressed or specialKeyPressed will only be called once when a key is first pressed).
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Register GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutSpecialUpFunc(specialKeyReleased);
	glutIdleFunc(idle);

	// Record when we started rendering the very first frame (which should happen after we call glutMainLoop).
	frameStartTime = (unsigned int)glutGet(GLUT_ELAPSED_TIME);

	// Enter the main drawing loop (this will never return).
	glutMainLoop();
}

/******************************************************************************
 * GLUT Callbacks (don't add any other functions here)
 ******************************************************************************/

 /*
	 Called when GLUT wants us to (re)draw the current animation frame.

	 Note: This function must not do anything to update the state of our simulated
	 world. Animation (moving or rotating things, responding to keyboard input,
	 etc.) should only be performed within the think() function provided below.
 */
void display(void)
{
	frameCount++;

	glShadeModel(GL_SMOOTH);

	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix into the model view matrix
	glLoadIdentity();

	//set up our camera - slightly up in the y so we can see the ground plane
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		dronePosition[0], dronePosition[1], dronePosition[2],
		0, 1, 0);

	drawOrigin();

	drawSky();

	//glPushMatrix();
	//drawBuilding(0.7, 0.7, 0.7, 3);
	//glPopMatrix();

	drawCity();

	//only apply the transforms inside the push/pop to the snowman and ground
	glPushMatrix();

	drawDrone();

	//draw the ground
	basicGround(GROUND_WIDTH, GROUND_LENGTH, GROUND_GRID);

	glPopMatrix();

	// swap the drawing buffers
	glutSwapBuffers();
}

/*
	Called when the OpenGL window has been resized.
*/
void reshape(int width, int h)
{
	windowHeight = h;
	windowWidth = width;

	glViewport(0, 0, windowWidth, windowHeight);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(60, (float)windowWidth / (float)windowHeight, 1, cameraZFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
	Called by GLUT when it's not rendering a frame.

	Note: We use this to handle animation and timing. You shouldn't need to modify
	this callback at all. Instead, place your animation logic (e.g. moving or rotating
	things) within the think() method provided with this template.
*/
void idle(void)
{
	// Wait until it's time to render the next frame.

	unsigned int frameTimeElapsed = (unsigned int)glutGet(GLUT_ELAPSED_TIME) - frameStartTime;
	if (frameTimeElapsed < FRAME_TIME)
	{
		// This frame took less time to render than the ideal FRAME_TIME: we'll suspend this thread for the remaining time,
		// so we're not taking up the CPU until we need to render another frame.
		unsigned int timeLeft = FRAME_TIME - frameTimeElapsed;
		Sleep(timeLeft);
	}

	// Begin processing the next frame.

	frameStartTime = glutGet(GLUT_ELAPSED_TIME); // Record when we started work on the new frame.

	think(); // Update our simulated world before the next call to display().

	glutPostRedisplay(); // Tell OpenGL there's a new frame ready to be drawn.
}

/******************************************************************************
 * Animation-Specific Functions (Add your own functions at the end of this section)
 ******************************************************************************/

 /*
	 Initialise OpenGL and set up our scene before we begin the render loop.
 */
void init(void)
{

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// set background color to be black
	glClearColor(0, 0, 0, 1.0);

	createSkyscraperDisplayList();

	initLights();

	initDroneCenterPosition();

	initCameraPosition();

	initSkyPosition();

	initShape();

	initTexture();

	newRandomDelay();
}

/*
	Advance our animation by FRAME_TIME milliseconds.

	Note: Our template's GLUT idle() callback calls this once before each new
	frame is drawn, EXCEPT the very first frame drawn after our application
	starts. Any setup required before the first frame is drawn should be placed
	in init().
*/
void think(void)
{
	thetaPropellar += 360 * FRAME_TIME_SEC; //360 degrees per second or 60 RPM
	lightTheta += lightDirection * (360 * FRAME_TIME_SEC);


	if (frameCount % lightDelay == 0)
	{
		lightDirection = -lightDirection;
		newRandomDelay();
	}


	movement();
}

/*
	Initialise OpenGL lighting before we begin the render loop.

	Note (advanced): If you're using dynamic lighting (e.g. lights that move around, turn on or
	off, or change colour) you may want to replace this with a drawLights function that gets called
	at the beginning of display() instead of init().
*/
/******************************************************************************/

void drawOrigin(void)
{
	glPushMatrix();

	glTranslated(GROUND_WIDTH / 2, 0, GROUND_LENGTH / 2);

	glColor3f(0.0f, 1.0f, 1.0f);
	glutWireSphere(0.1, 10, 10);
	glBegin(GL_LINES);

	//x axis -red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 0.0f);

	//y axis -green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);

	//z axis - blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 2.0f);

	glEnd();

	glPopMatrix();
}