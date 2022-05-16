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
 * Some Simple Definitions of Motion
 ******************************************************************************/

#define MOTION_NONE 0				// No motion.
#define MOTION_CLOCKWISE -1			// Clockwise rotation.
#define MOTION_ANTICLOCKWISE 1		// Anticlockwise rotation.
#define MOTION_BACKWARD -1			// Backward motion.
#define MOTION_FORWARD 1			// Forward motion.
#define MOTION_LEFT -1				// Leftward motion.
#define MOTION_RIGHT 1				// Rightward motion.
#define MOTION_DOWN -1				// Downward motion.
#define MOTION_UP 1					// Upward motion.

 // Represents the motion of an object on four axes (Yaw, Surge, Sway, and Heave).
 // 
 // You can use any numeric values, as specified in the comments for each axis. However,
 // the MOTION_ definitions offer an easy way to define a "unit" movement without using
 // magic numbers (e.g. instead of setting Surge = 1, you can set Surge = MOTION_FORWARD).
 //
typedef struct {
	int Yaw;		// Turn about the Z axis	[<0 = Clockwise, 0 = Stop, >0 = Anticlockwise]
	int Surge;		// Move forward or back		[<0 = Backward,	0 = Stop, >0 = Forward]
	int Sway;		// Move sideways (strafe)	[<0 = Left, 0 = Stop, >0 = Right]
	int Heave;		// Move vertically			[<0 = Down, 0 = Stop, >0 = Up]
	int Pitch;
	int Roll;
} motionstate4_t;

/******************************************************************************
 * Keyboard Input Handling Setup
 ******************************************************************************/

// Represents the state of a single keyboard key.Represents the state of a single keyboard key.
typedef enum {
	KEYSTATE_UP = 0,	// Key is not pressed.
	KEYSTATE_DOWN		// Key is pressed down.
} keystate_t;

// Represents the states of a set of keys used to control an object's motion.
typedef struct {
	keystate_t MoveForward;
	keystate_t MoveBackward;
	keystate_t MoveLeft;
	keystate_t MoveRight;
	keystate_t MoveUp;
	keystate_t MoveDown;
	keystate_t TurnLeft;
	keystate_t TurnRight;
	keystate_t TurnUp;
	keystate_t TurnDown;
	keystate_t TurnClockwise;
	keystate_t TurnAnticlockwise;
} motionkeys_t;

// Current state of all keys used to control our "player-controlled" object's motion.
motionkeys_t motionKeyStates = {
	KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP,
	KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP,
	KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP, KEYSTATE_UP };

// How our "player-controlled" object should currently be moving, solely based on keyboard input.
//
// Note: this may not represent the actual motion of our object, which could be subject to
// other controls (e.g. mouse input) or other simulated forces (e.g. gravity).
motionstate4_t keyboardMotion = { MOTION_NONE, MOTION_NONE, MOTION_NONE, MOTION_NONE, MOTION_NONE };

// Define all character keys used for input (add any new key definitions here).
// Note: USE ONLY LOWERCASE CHARACTERS HERE. The keyboard handler provided converts all
// characters typed by the user to lowercase, so the SHIFT key is ignored.

#define KEY_MOVE_FORWARD	'w'
#define KEY_MOVE_BACKWARD	's'
#define KEY_MOVE_LEFT		'a'
#define KEY_MOVE_RIGHT		'd'
#define KEY_EXIT			27 // Escape key.
#define KEY_MOVE_UP ' '
#define KEY_TURN_CLOCKWISE 'e'
#define KEY_TURN_ANTICLOCKWISE 'q'


// Define all GLUT special keys used for input (add any new key definitions here).

#define SP_KEY_MOVE_DOWN	GLUT_KEY_SHIFT_L
#define SP_KEY_TURN_LEFT	GLUT_KEY_LEFT
#define SP_KEY_TURN_RIGHT	GLUT_KEY_RIGHT
#define SP_KEY_TURN_UP		GLUT_KEY_UP
#define SP_KEY_TURN_DOWN	GLUT_KEY_DOWN


/******************************************************************************
 * GLUT Callback Prototypes
 ******************************************************************************/

void display(void);
void reshape(int width, int h);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
void keyReleased(unsigned char key, int x, int y);
void specialKeyReleased(int key, int x, int y);
void idle(void);

/******************************************************************************
 * Animation-Specific Function Prototypes (add your own here)
 ******************************************************************************/

void main(int argc, char **argv);
void init(void);
void think(void);
void initLights(void);
void initCameraPosition(void);
void initDroneCenterPosition(void);

void drawDrone(void);
void drawBody(void);
void drawArm(int direction);
void drawLeg(void);
void drawPropeller(void);
void drawOrigin(void);
void basicGround(void);

/******************************************************************************
 * Animation-Specific Setup (Add your own definitions, constants, and globals here)
 ******************************************************************************/
// Angle tools
#define PI 3.14159265359
#define DEG_TO_RAD PI/180

// Render objects as filled polygons (1) or wireframes (0). Default filled.
int renderFillEnabled = 1;

// drawing propeller: sides for drawArm
enum Side {
	frontLeftSide = -2,
	frontRightSide = -1,
	rearLeftSide = 1,
	rearRightSide = 2,
};

// window dimensions
GLint windowWidth = 1280;
GLint windowHeight = 720;

// Camera
GLfloat cameraPosition[] = { 0, 1, 15 };

float cameraAngle = 0.0f;
float cameraHeading = 0.0f;
float cameraDistanceXY = 10.f;

//#define 


// pointer to quadric objects
GLUquadricObj *sphereQuadric;
GLUquadricObj *cylinderQuadric;

//drone hierachical model setup values
//dimensions of the body
#define BODY_RADIUS 1.0
#define BODY_Y_SCALE 0.5

GLfloat dronePosition[3] = { 0.0f, 0.0f, 0.0f };
const float droneSpeed = 8.0f; // Metres per second

float droneYawHeading = 0.0; // degrees in facing direction
float dronePitchHeading = 0.0f; // degrees in facing direction
float droneRollHeading = 0.0f; // degrees in facing direction

// arm dimensions
#define DRONE_ARM_LENGTH 1.1
#define DRONE_UPRIGHT_LEGTH 1.0
#define DRONE_ARM_WIDTH 0.06

// propellar dimensions
#define PROPELLER_LENGTH 1.1
#define PROPELLER_WIDTH 0.06

float thetaPropellar = 0.0f;

// ground
#define GROUND_WIDTH 250
#define GROUND_LENGTH 250
#define GROUND_GRID 10


/******************************************************************************
 * Entry Point (don't put anything except the main function here)
 ******************************************************************************/

void main(int argc, char **argv)
{
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
	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix into the model view matrix
	glLoadIdentity();

	//set up our camera - slightly up in the y so we can see the ground plane
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		dronePosition[0], dronePosition[1], dronePosition[2],
		0, 1, 0);

	drawOrigin();

	glColor3f(1.0f, 1.0f, 1.0f);

	//only apply the transforms inside the push/pop to the snowman and ground
	glPushMatrix();

	drawDrone();

	//draw the ground
	basicGround();

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

	gluPerspective(60, (float)windowWidth / (float)windowHeight, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is pressed.
*/
void keyPressed(unsigned char key, int x, int y)
{
	switch (tolower(key)) {

		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			Whenever one of our movement keys is pressed, we do two things:
			(1) Update motionKeyStates to record that the key is held down. We use
				this later in the keyReleased callback.
			(2) Update the relevant axis in keyboardMotion to set the new direction
				we should be moving in. The most recent key always "wins" (e.g. if
				you're holding down KEY_MOVE_LEFT then also pressed KEY_MOVE_RIGHT,
				our object will immediately start moving right).
		*/
	case KEY_MOVE_FORWARD:
		motionKeyStates.MoveForward = KEYSTATE_DOWN;
		keyboardMotion.Surge = MOTION_FORWARD;
		break;
	case KEY_MOVE_BACKWARD:
		motionKeyStates.MoveBackward = KEYSTATE_DOWN;
		keyboardMotion.Surge = MOTION_BACKWARD;
		break;
	case KEY_MOVE_LEFT:
		motionKeyStates.MoveLeft = KEYSTATE_DOWN;
		keyboardMotion.Sway = MOTION_LEFT;
		break;
	case KEY_MOVE_RIGHT:
		motionKeyStates.MoveRight = KEYSTATE_DOWN;
		keyboardMotion.Sway = MOTION_RIGHT;
		break;
	case KEY_MOVE_UP:
		motionKeyStates.MoveUp = KEYSTATE_DOWN;
		keyboardMotion.Heave = MOTION_UP;
		break;
	case KEY_TURN_CLOCKWISE:
		motionKeyStates.TurnClockwise = KEYSTATE_DOWN;
		keyboardMotion.Roll = MOTION_CLOCKWISE;
		break;
	case KEY_TURN_ANTICLOCKWISE:
		motionKeyStates.TurnAnticlockwise = KEYSTATE_DOWN;
		keyboardMotion.Roll = MOTION_ANTICLOCKWISE;
		break;

		/*
			Other Keyboard Functions (add any new character key controls here)

			Rather than using literals (e.g. "t" for spotlight), create a new KEY_
			definition in the "Keyboard Input Handling Setup" section of this file.
			For example, refer to the existing keys used here (KEY_MOVE_FORWARD,
			KEY_MOVE_LEFT, KEY_EXIT, etc).
		*/
	case KEY_EXIT:
		exit(0);
		break;
	}
}

/*
	Called each time a "special" key (e.g. an arrow key) is pressed.
*/
void specialKeyPressed(int key, int x, int y)
{
	switch (key) {

		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			This works as per the motion keys in keyPressed.
		*/
	case SP_KEY_MOVE_DOWN:
		motionKeyStates.MoveDown = KEYSTATE_DOWN;
		keyboardMotion.Heave = MOTION_DOWN;
		break;
	case SP_KEY_TURN_LEFT:
		motionKeyStates.TurnLeft = KEYSTATE_DOWN;
		keyboardMotion.Yaw = MOTION_ANTICLOCKWISE;
		break;
	case SP_KEY_TURN_RIGHT:
		motionKeyStates.TurnRight = KEYSTATE_DOWN;
		keyboardMotion.Yaw = MOTION_CLOCKWISE;
		break;
	case SP_KEY_TURN_UP:
		motionKeyStates.TurnUp = KEYSTATE_DOWN;
		keyboardMotion.Pitch = MOTION_CLOCKWISE;
		break;
	case SP_KEY_TURN_DOWN:
		motionKeyStates.TurnDown = KEYSTATE_DOWN;
		keyboardMotion.Pitch = MOTION_ANTICLOCKWISE;
		break;

		/*
			Other Keyboard Functions (add any new special key controls here)

			Rather than directly using the GLUT constants (e.g. GLUT_KEY_F1), create
			a new SP_KEY_ definition in the "Keyboard Input Handling Setup" section of
			this file. For example, refer to the existing keys used here (SP_KEY_MOVE_UP,
			SP_KEY_TURN_LEFT, etc).
		*/
	}
}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is released.
*/
void keyReleased(unsigned char key, int x, int y)
{
	switch (tolower(key)) {

		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			Whenever one of our movement keys is released, we do two things:
			(1) Update motionKeyStates to record that the key is no longer held down;
				we need to know when we get to step (2) below.
			(2) Update the relevant axis in keyboardMotion to set the new direction
				we should be moving in. This gets a little complicated to ensure
				the controls work smoothly. When the user releases a key that moves
				in one direction (e.g. KEY_MOVE_RIGHT), we check if its "opposite"
				key (e.g. KEY_MOVE_LEFT) is pressed down. If it is, we begin moving
				in that direction instead. Otherwise, we just stop moving.
		*/
	case KEY_MOVE_FORWARD:
		motionKeyStates.MoveForward = KEYSTATE_UP;
		keyboardMotion.Surge = (motionKeyStates.MoveBackward == KEYSTATE_DOWN) ? MOTION_BACKWARD : MOTION_NONE;
		break;
	case KEY_MOVE_BACKWARD:
		motionKeyStates.MoveBackward = KEYSTATE_UP;
		keyboardMotion.Surge = (motionKeyStates.MoveForward == KEYSTATE_DOWN) ? MOTION_FORWARD : MOTION_NONE;
		break;
	case KEY_MOVE_LEFT:
		motionKeyStates.MoveLeft = KEYSTATE_UP;
		keyboardMotion.Sway = (motionKeyStates.MoveRight == KEYSTATE_DOWN) ? MOTION_RIGHT : MOTION_NONE;
		break;
	case KEY_MOVE_RIGHT:
		motionKeyStates.MoveRight = KEYSTATE_UP;
		keyboardMotion.Sway = (motionKeyStates.MoveLeft == KEYSTATE_DOWN) ? MOTION_LEFT : MOTION_NONE;
		break;
	case KEY_MOVE_UP:
		motionKeyStates.MoveUp = KEYSTATE_UP;
		keyboardMotion.Heave = (motionKeyStates.MoveDown == KEYSTATE_DOWN) ? MOTION_DOWN : MOTION_NONE;
		break;
	case KEY_TURN_CLOCKWISE:
		motionKeyStates.TurnClockwise = KEYSTATE_UP;
		keyboardMotion.Roll = (motionKeyStates.TurnClockwise == KEYSTATE_DOWN) ? MOTION_CLOCKWISE : MOTION_NONE;
		break;
	case KEY_TURN_ANTICLOCKWISE:
		motionKeyStates.TurnAnticlockwise = KEYSTATE_UP;
		keyboardMotion.Roll = (motionKeyStates.TurnAnticlockwise == KEYSTATE_DOWN) ? MOTION_ANTICLOCKWISE : MOTION_NONE;
		break;
	

		/*
			Other Keyboard Functions (add any new character key controls here)

			Note: If you only care when your key is first pressed down, you don't have to
			add anything here. You only need to put something in keyReleased if you care
			what happens when the user lets go, like we do with our movement keys above.
			For example: if you wanted a spotlight to come on while you held down "t", you
			would need to set a flag to turn the spotlight on in keyPressed, and update the
			flag to turn it off in keyReleased.
		*/
	}
}

/*
	Called each time a "special" key (e.g. an arrow key) is released.
*/
void specialKeyReleased(int key, int x, int y)
{
	switch (key) {
		/*
			Keyboard-Controlled Motion Handler - DON'T CHANGE THIS SECTION

			This works as per the motion keys in keyReleased.
		*/
	case SP_KEY_MOVE_DOWN:
		motionKeyStates.MoveDown = KEYSTATE_UP;
		keyboardMotion.Heave = (motionKeyStates.MoveUp == KEYSTATE_DOWN) ? MOTION_UP : MOTION_NONE;
		break;
	case SP_KEY_TURN_LEFT:
		motionKeyStates.TurnLeft = KEYSTATE_UP;
		keyboardMotion.Yaw = (motionKeyStates.TurnRight == KEYSTATE_DOWN) ? MOTION_CLOCKWISE : MOTION_NONE;
		break;
	case SP_KEY_TURN_RIGHT:
		motionKeyStates.TurnRight = KEYSTATE_UP;
		keyboardMotion.Yaw = (motionKeyStates.TurnLeft == KEYSTATE_DOWN) ? MOTION_ANTICLOCKWISE : MOTION_NONE;
		break;
	case SP_KEY_TURN_UP:
		motionKeyStates.TurnUp = KEYSTATE_UP;
		keyboardMotion.Pitch = (motionKeyStates.TurnUp == KEYSTATE_DOWN) ? MOTION_CLOCKWISE : MOTION_NONE;
		break;
	case SP_KEY_TURN_DOWN:
		motionKeyStates.TurnDown = KEYSTATE_UP;
		keyboardMotion.Pitch = (motionKeyStates.TurnDown == KEYSTATE_DOWN) ? MOTION_ANTICLOCKWISE : MOTION_NONE;
		break;

		/*
			Other Keyboard Functions (add any new special key controls here)

			As per keyReleased, you only need to handle the key here if you want something
			to happen when the user lets go. If you just want something to happen when the
			key is first pressed, add you code to specialKeyPressed instead.
		*/
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

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// set background color to be black
	glClearColor(0, 0, 0, 1.0);


	initLights();

	initDroneCenterPosition();

	initCameraPosition();

	//create the quadric for drawing the sphere
	sphereQuadric = gluNewQuadric();

	//create the quadric for drawing the cylinder
	cylinderQuadric = gluNewQuadric();
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

	/*
		Keyboard motion handler: complete this section to make your "player-controlled"
		object respond to keyboard input.
	*/
	if (keyboardMotion.Roll != MOTION_NONE)
	{
		droneRollHeading += keyboardMotion.Roll * 360.0f * FRAME_TIME_SEC; //60 RPM
		if (droneRollHeading >= 360)
			droneRollHeading = 0;
		else if (droneRollHeading <= 0)
			droneRollHeading = 360;
		printf("Drone Roll Heading: %f\n", droneRollHeading);
	}
	if (keyboardMotion.Pitch != MOTION_NONE)
	{
		dronePitchHeading += keyboardMotion.Pitch * 360.0f * FRAME_TIME_SEC; //60 RPM
		if (dronePitchHeading >= 360)
			dronePitchHeading = 0;
		else if (dronePitchHeading <= 0)
			dronePitchHeading = 360;
		printf("Drone Pitch Heading: %f\n", dronePitchHeading);
	}
	if (keyboardMotion.Yaw != MOTION_NONE) {
		droneYawHeading += keyboardMotion.Yaw * 360.0f * FRAME_TIME_SEC; //60 RPM
		if (droneYawHeading >= 360)
			droneYawHeading = 0;
		else if (droneYawHeading <= 0)
			droneYawHeading = 360;
		printf("Drone Yaw Heading: %f\n", droneYawHeading);

		cameraPosition[0] = dronePosition[0] + ((float)sin((droneYawHeading - cameraAngle) * DEG_TO_RAD)) * cameraDistanceXY;
		cameraPosition[2] = dronePosition[2] + ((float)cos((droneYawHeading - cameraAngle) * DEG_TO_RAD)) * cameraDistanceXY;
	}
	if (keyboardMotion.Surge != MOTION_NONE) {
		dronePosition[0] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC); //20 m/sec
		dronePosition[2] -= (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC); //20 m/sec

		cameraPosition[0] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC);
		cameraPosition[2] -= (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC);
	}
	if (keyboardMotion.Sway != MOTION_NONE) {
		dronePosition[0] += (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC); //20 m/sec
		dronePosition[2] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC); //20 m/sec

		cameraPosition[0] += (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC);
		cameraPosition[2] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC);
	}
	if (keyboardMotion.Heave != MOTION_NONE) {
		dronePosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC; //20 m/sec
		cameraPosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC;
	}
}

/*
	Initialise OpenGL lighting before we begin the render loop.

	Note (advanced): If you're using dynamic lighting (e.g. lights that move around, turn on or
	off, or change colour) you may want to replace this with a drawLights function that gets called
	at the beginning of display() instead of init().
*/
void initLights(void)
{
	// Simple lighting setup
	GLfloat globalAmbient[] = { 0.4f, 0.4f, 0.4f, 1 };
	GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	GLfloat ambientLight[] = { 0, 0, 0, 1 };
	GLfloat diffuseLight[] = { 1, 1, 1, 1 };
	GLfloat specularLight[] = { 1, 1, 1, 1 };

	// Configure global ambient lighting.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Configure Light 0.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Make GL normalize the normal vectors we supply.
	glEnable(GL_NORMALIZE);

	// Enable use of simple GL colours as materials.
	glEnable(GL_COLOR_MATERIAL);
}

void initCameraPosition(void)
{
	cameraPosition[0] = dronePosition[0] + ((float)sin((droneYawHeading - cameraAngle) * DEG_TO_RAD)) * cameraDistanceXY;
	cameraPosition[1] = 2.5;
	cameraPosition[2] = dronePosition[2] + ((float)cos((droneYawHeading - cameraAngle) * DEG_TO_RAD)) * cameraDistanceXY;
}

void initDroneCenterPosition(void)
{
	dronePosition[0] = GROUND_WIDTH / 2;
	dronePosition[1] = 0.f;
	dronePosition[2] = GROUND_LENGTH / 2;
}

/******************************************************************************/

/*
  A simple ground plane in the XZ plane with vertex normals specified for lighting
  the top face of the ground. The bottom face is not lit.
*/

void basicGround(void)
{
	glColor3d(0.8, 0.9, 1);

	glPushMatrix();
	glTranslated(0, -BODY_RADIUS / 2, 0); //shifted this so looks like in snow

	for (int i = 0; i < GROUND_WIDTH; i+=10)
	{
		for(int j = 0; j < GROUND_LENGTH; j+= 10)
		{
			glBegin(GL_QUADS);
			glNormal3d(0, 1, 0);
			glVertex3d(i, 0, j);
			glNormal3d(0, 1, 0);
			glVertex3d(i + GROUND_GRID, 0, j);
			glNormal3d(0, 1, 0);
			glVertex3d(i + GROUND_GRID, 0, j + GROUND_GRID);
			glNormal3d(0, 1, 0);
			glVertex3d(i, 0, j + GROUND_GRID);
			glEnd();
		}
	}

	glPopMatrix();
}

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

void drawDrone(void)
{
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	// moving the drone
	glTranslatef(dronePosition[0], dronePosition[1], dronePosition[2]);
	glRotated(droneYawHeading, 0, 1, 0); // yaw rotate
	glRotated(dronePitchHeading, 1, 0, 0); // pitch rotate
	glRotated(droneRollHeading, 0, 0, 1); // roll rotate

	// draw the body
	drawBody();

	//drawLeg();

	glPushMatrix();
	glRotated(45, 0, 1, 0);

	// left side arm
	glPushMatrix();
	glTranslated(-(BODY_RADIUS * 1.5), 0, 0);
	drawArm(1);
	glPopMatrix();

	// right side arm
	glPushMatrix();
	glTranslated(BODY_RADIUS * 1.5, 0, 0);
	glRotated(180, 0, 1, 0);
	drawArm(1);
	glPopMatrix();

	// front arm
	glPushMatrix();
	glTranslated(0, 0, BODY_RADIUS * 1.5);
	glRotated(90, 0, 1, 0);
	drawArm(-1);
	glPopMatrix();

	// back arm
	glPushMatrix();
	glTranslated(0, 0, -(BODY_RADIUS * 1.5));
	glRotated(-90, 0, 1, 0);
	drawArm(-1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void drawBody(void)
{
	glPushMatrix();

	glPushMatrix();
	// squash the y axis
	glScaled(BODY_RADIUS, BODY_Y_SCALE, BODY_RADIUS);

	gluSphere(sphereQuadric, BODY_RADIUS, 50, 50);

	glTranslated(0.0, BODY_RADIUS, 0.0);
	glPopMatrix();

	//glPushMatrix();
	//glTranslated(0, 0, -BODY_RADIUS * 1.1);
	//gluSphere(sphereQuadric, 0.3, 10, 10);
	//glPopMatrix();

	glPopMatrix();
}

void drawArm(int direction)
{
	glPushMatrix();

	// bottom arm
	//glTranslated(-(DRONE_ARM_LENGTH / 2.0), 0, 0);

	glPushMatrix();
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(cylinderQuadric, DRONE_ARM_WIDTH, DRONE_ARM_WIDTH * 1.5, DRONE_ARM_LENGTH, 10, 10);
	glPopMatrix();

	// arm cap
	glPushMatrix();
	gluSphere(sphereQuadric, DRONE_ARM_WIDTH * 1.5, 10, 10);
	glPopMatrix();

	// up-right arm

	glPushMatrix();
	glRotated(90, -1, 0, 0);
	gluCylinder(cylinderQuadric, DRONE_ARM_WIDTH, DRONE_ARM_WIDTH, DRONE_ARM_LENGTH / 4, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, DRONE_ARM_LENGTH / 4, 0);
	drawPropeller();
	glPopMatrix();

	glPopMatrix();
}

void drawLeg(void)
{
	glPushMatrix();

	glTranslated(BODY_RADIUS * 0.4, -BODY_Y_SCALE * 0.75, BODY_RADIUS * 0.4);
	glRotated(90, 1, 0, 0);
	//gluCylinder(cylinderQuadric, DRONE_ARM_WIDTH, DRONE_ARM_WIDTH, DRONE_ARM_LENGTH / 4, 10, 10);
	glPushMatrix();
	gluSphere(sphereQuadric, DRONE_ARM_WIDTH, 10, 10);
	glPopMatrix();
	glPopMatrix();
}

void drawPropeller(void)
{
	glPushMatrix();

	glRotated(thetaPropellar, 0, 1, 0);

	// draw first propeller wing
	glPushMatrix();
	glTranslated(-(PROPELLER_LENGTH / 2.0), 0, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(cylinderQuadric, PROPELLER_WIDTH, PROPELLER_WIDTH, PROPELLER_LENGTH, 10, 10);
	glPopMatrix();

	// draw second propeller wing
	glPushMatrix();
	glTranslated(0, 0, -(PROPELLER_LENGTH / 2.0));
	gluCylinder(cylinderQuadric, PROPELLER_WIDTH, PROPELLER_WIDTH, PROPELLER_LENGTH, 10, 10);
	glPopMatrix();

	//draw the first propellar end caps
	glPushMatrix();
	glTranslated(-(PROPELLER_LENGTH / 2), 0.0, 0.0);
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(PROPELLER_LENGTH / 2, 0.0, 0.0);
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	// draw the second propellar end caps
	glPushMatrix();
	glTranslated(0.0, 0.0, -(PROPELLER_LENGTH / 2));
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.0, PROPELLER_LENGTH / 2);
	gluSphere(sphereQuadric, PROPELLER_WIDTH, 10, 10);
	glPopMatrix();

	//draw the propellar center
	glPushMatrix();
	gluSphere(sphereQuadric, PROPELLER_WIDTH * 1.5, 6, 6);
	glPopMatrix();

	glPopMatrix();
}