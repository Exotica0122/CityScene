#include "Input.h"

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

void movement(void)
{
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

	// Move Forward backward
	if (keyboardMotion.Surge != MOTION_NONE) {
		dronePosition[0] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC); //20 m/sec
		dronePosition[2] -= (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC); //20 m/sec

		cameraPosition[0] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC);
		cameraPosition[2] -= (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC);

		skyPosition[0] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC);
		skyPosition[2] -= (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Surge * droneSpeed * FRAME_TIME_SEC);
	}

	// Move left right
	if (keyboardMotion.Sway != MOTION_NONE) {
		dronePosition[0] += (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC); //20 m/sec
		dronePosition[2] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC); //20 m/sec

		cameraPosition[0] += (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC);
		cameraPosition[2] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC);

		skyPosition[0] += (cos(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC);
		skyPosition[2] -= (sin(droneYawHeading * DEG_TO_RAD) * keyboardMotion.Sway * droneSpeed * FRAME_TIME_SEC);
	}

	// Move down until ground
	// && dronePosition[1] > (BODY_RADIUS / 2)
	if (keyboardMotion.Heave == MOTION_DOWN && dronePosition[1] > (BODY_RADIUS / 2))
	{
		dronePosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC; //20 m/sec
		cameraPosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC;
		//skyPosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC;
	}

	// Move up
	if (keyboardMotion.Heave == MOTION_UP)
	{
		dronePosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC; //20 m/sec
		cameraPosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC;
		//skyPosition[1] += keyboardMotion.Heave * droneSpeed * FRAME_TIME_SEC;
	}
}