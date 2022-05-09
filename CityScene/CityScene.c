/******************************************************************************
 *
 * Animation v1.0 (23/02/2021)
 *
 * This template provides a basic FPS-limited render loop for an animated scene.
 *
 ******************************************************************************/

#include <Windows.h>
#include <freeglut.h>
#include <math.h>
#include <stdio.h>


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

/******************************************************************************
 * Keyboard Input Handling Setup
 ******************************************************************************/

 // Define all character keys used for input (add any new key definitions here).
 // Note: USE ONLY LOWERCASE CHARACTERS HERE. The keyboard handler provided converts all
 // characters typed by the user to lowercase, so the SHIFT key is ignored.

#define KEY_BASE			'1' // Select arm base.
#define KEY_LOWER_ARM		'2' // Select lower arm joint.
#define KEY_UPPER_ARM		'3' // Select upper arm joint.
#define KEY_ANGLE_INC		'a' // Increment angle (rotate joint anti-clockwise)
#define KEY_ANGLE_DEC		'd' // Decrement angle (rotate joint clockwise)
#define KEY_EXIT			27	// Escape key.

/******************************************************************************
 * GLUT Callback Prototypes
 ******************************************************************************/

void display(void);
void reshape(int width, int h);
void keyPressed(unsigned char key, int x, int y);
void keyReleased(unsigned char key, int x, int y);
void idle(void);

/******************************************************************************
 * Animation-Specific Function Prototypes (add your own here)
 ******************************************************************************/

void main(int argc, char **argv);
void init(void);
void think(void);

void base(void);
void armSegment(float armWidth, float armHeight);

/******************************************************************************
 * Animation-Specific Setup (Add your own definitions, constants, and globals here)
 ******************************************************************************/

 // dimensions of the base
#define BASE_HEIGHT 2.0
#define BASE_RADIUS 1.0

// dimensions of the lower arm
#define LOWER_ARM_HEIGHT 5.0
#define LOWER_ARM_WIDTH  0.5

// dimensions of the upper arm
#define UPPER_ARM_HEIGHT 3.0
#define UPPER_ARM_WIDTH  0.5

// arm joints (this map to the indices in angles[])
#define JOINT_BASE 0
#define JOINT_LOWER_ARM 1
#define JOINT_UPPER_ARM 2

// joint motion directions
#define MOVE_NONE 0			// Joint isn't moving
#define MOVE_ANGLE_INC 1	// Increment angle (rotate anti-clockwise)
#define MOVE_ANGLE_DEC -1	// Decrement angle (rotate clockwise)

// joint rotation speed
const float JOINT_ROTATION_SPEED = 40.0f; // degrees per second

// three angles for each of the threes joints in the robot
GLfloat angles[] = { 0.0, 0.0, 0.0 };

// current joint  (JOINT_BASE, JOINT_LOWER_ARM, or JOINT_UPPER_ARM)
GLint joint = JOINT_BASE;

// direction the current joint is moving (MOVE_NONE, MOVE_ANGLE_DEC, or MOVE_ANGLE_INC)
GLint jointMoveDir = MOVE_NONE;

// pointer to quadric object
GLUquadricObj  *myQuadric;

/******************************************************************************
 * Entry Point (don't put anything except the main function here)
 ******************************************************************************/

void main(int argc, char **argv)
{
	// Initialize the OpenGL window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Animation");

	// Set up the scene.
	init();

	// Disable key repeat (keyPressed or specialKeyPressed will only be called once when a key is first pressed).
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Register GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
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
	// clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// start with a fresh transformation matrix
	glLoadIdentity();

	// rotate the base and draw it  (notice that we don't use push and pop)
	glRotatef(angles[0], 0.0, 1.0, 0.0);
	glColor3f(joint == JOINT_BASE ? 1.0f : 0.5f, 0.0, 0.0);
	base();

	glTranslated(0.0, BASE_HEIGHT, 0.0);
	glRotatef(angles[1], 0.0, 0.0, 1.0);
	glColor3f(joint == JOINT_LOWER_ARM ? 1.0f : 0.5f, 0.0, 0.0);
	armSegment(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT);

	// rotate the upper arm, move it to the end of the lower arm and draw it
	glTranslatef(0.0, LOWER_ARM_HEIGHT, 0.0);
	glRotatef(angles[2], 0.0, 0.0, 1.0);
	glColor3f(joint == JOINT_UPPER_ARM ? 1.0f : 0.5f, 0.0, 0.0);
	armSegment(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT);


	glutSwapBuffers();
}

/*
	Called when the OpenGL window has been resized.
*/
void reshape(int width, int h)
{
	// set the viewport
	glViewport(0, 0, width, h);

	// set the orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width <= h)
		glOrtho(-10.0, 10.0,
			-5.0 * (GLfloat)h / (GLfloat)width, 15.0 * (GLfloat)h / (GLfloat)width,
			-10.0, 10.0);
	else
		glOrtho(-10.0 * (GLfloat)width / (GLfloat)h, 10.0 * (GLfloat)width / (GLfloat)h,
			-5.0, 15.0,
			-10.0, 10.0);

	// return to model view mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is pressed.
*/
void keyPressed(unsigned char key, int x, int y)
{
	switch (tolower(key)) {
	case KEY_BASE:
		joint = JOINT_BASE;
		break;
	case KEY_LOWER_ARM:
		joint = JOINT_LOWER_ARM;
		break;
	case KEY_UPPER_ARM:
		joint = JOINT_UPPER_ARM;
		break;
	case KEY_ANGLE_DEC:
		jointMoveDir = MOVE_ANGLE_DEC;
		break;
	case KEY_ANGLE_INC:
		jointMoveDir = MOVE_ANGLE_INC;
		break;
	case KEY_EXIT:
		exit(0);
		break;
	}
}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is released.
*/
void keyReleased(unsigned char key, int x, int y)
{
	switch (tolower(key)) {
	case KEY_ANGLE_DEC:
	case KEY_ANGLE_INC:
		jointMoveDir = MOVE_NONE;
		break;

	}
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
	// set the clear color and the drawing color
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// create a new quadric for drawing the cylinder
	myQuadric = gluNewQuadric();
	// render it as wireframe object
	gluQuadricDrawStyle(myQuadric, GLU_LINE);
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
	if (jointMoveDir != MOVE_NONE) {
		// move the current joint in the right direction (jointMoveDir) at our predefined constant speed
		angles[joint] = fmodf(angles[joint] + jointMoveDir * JOINT_ROTATION_SPEED * FRAME_TIME_SEC, 360.0f);
	}
}

void base(void)
{
	glPushMatrix();

	// rotate cylinder to align with y axis (originally aligned with the Z axis) 
	glRotatef(-90.0, 1.0, 0.0, 0.0);

	// cyliner aligned with z axis, render with
	// 10 slices for base and 10 along length 
	gluCylinder(myQuadric, BASE_RADIUS, BASE_RADIUS, BASE_HEIGHT, 10, 10);

	glPopMatrix();
}

void armSegment(float armWidth, float armHeight)
{
	glPushMatrix();
	// move the arm to be above the origin
	glTranslatef(0.0f, 0.5f * armHeight, 0.0f);

	// scale the arm so that it is long and thin
	glScalef(armWidth, armHeight, armWidth);

	// draw the arm
	glutWireCube(1.0);
	glPopMatrix();
}


/******************************************************************************/