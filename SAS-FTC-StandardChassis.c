#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     LDriveMotor,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     RDriveMotor,   tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     FuncMotorA,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     FuncMotorB,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    Servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    Servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    Servo3,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                     Singapore American School, One Degree North
//
//                               Program, Teleop, FTC, 2013
//                               Standard Tank Drive, Mk. 1
//
// Standard chassis tank drive teleop program. To be used on a standard NXT + TETRIX kitbot for the FTC
// competition. Must be modified to include other functions.
//
// Code provided to keep the coding process streamlined and focused on mission-specific code and
// keep chassis the same every year.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c" //Teleop Driver


//BEGIN PROGRAM FLAGS
//0 for true, 1 for false
#define JOYSTICK_DEADZONE 0 //enable deadzoning
#define JOYSTICK_LIN_SCALING 0 //enable linear scaling

#define ROBOT_LIMIT_FWD_ACCELERATION 0 //limit motor so change in power over time is limited
#define ROBOT_HALF_VELOCITY_LIMIT 0 //limit total power as a certain value
//END PROGRAM FLAGS

//BEGIN PROGRAM OPTIONS
#define JOYSTICK_DEADZONE_SIZE 2
#define ROBOT_ACCELERATION_LIMIT_SENSITIVITY 3 //Limits d/dt Duty Cycle (in percent)
//END PROGRAM OPTIONS

//button definitions
#define button1   0x01
#define button2   0x02
#define button3   0x04
#define button4   0x08
#define button5   0x10
#define button6   0x20
#define button7   0x40
#define button8   0x80
#define button9  0x100
#define button10 0x200





short jy1_p = 0, jy2_p = 0; //Values of y1 and y2 used previously, required for acc limit

//Filters joystick data and returns motor power output, scaled accordingly
short filter(short jValue) {
	short j = jValue;

	if(JOYSTICK_DEADZONE && j < JOYSTICK_DEADZONE_SIZE && j > JOYSTICK_DEADZONE_SIZE) //Apply joystick deadzoning
		j=0;

	if(!JOYSTICK_LIN_SCALING) { //"Exponential" scaling
		float temp = ((float)j)/127.0;
		j=(int)(100.001*temp*temp*temp); //y = 100.1 * (x/127)^3; 0.1 to ensure; close approximation to exponential function
	}
	else {
		j*=100.0/127.0;	//Linear scaling to -100 ~ 100
	}

	return j;
}

//Gets joystick values, filters them, applies pwm cycle adjustments, and sends to motors.
void drive(short jy1, short jy2) {
	jy1 = filter(jy1);
	jy2 = filter(jy2);

	if(ROBOT_LIMIT_FWD_ACCELERATION) {
		int delta = jy1 - jy1_p;

	if(ROBOT_HALF_VELOCITY_LIMIT) {
		jy1 /= 2;
		jy2 /= 2;
	}

		if((delta) * (jy1) > 0 && abs(delta) > ROBOT_ACCELERATION_LIMIT_SENSITIVITY ) //If robot is accelerating the direction of velocity (sign of jy1-jy1_p and jy1 is same)
			jy1 = (abs(delta)/delta)*ROBOT_ACCELERATION_LIMIT_SENSITIVITY + jy1_p;

		delta = jy2 - jy2_p;

		if((delta) * (jy2) > 0 && abs(delta) > ROBOT_ACCELERATION_LIMIT_SENSITIVITY ) //If robot is accelerating the direction of velocity (sign of jy2-jy2_p and jy2 is same)
			jy2 = (abs(delta)/delta)*ROBOT_ACCELERATION_LIMIT_SENSITIVITY + jy2_p;

		jy1_p = jy1;
		jy2_p = jy2;
	}

	motor[LDriveMotor] = jy1;
	motor[RDriveMotor] = jy2;

}

short hatAction() { //Checks hat and does arcade drive if clicked. Returns zero if pad is not pressed, non-zero if some direction is pressed.

	switch(joystick.joy1_TopHat) {
		case -1: //not pressed
			return 0;

		case 0: //forward
			motor[LDriveMotor] = 100;
			motor[RDriveMotor] = 100;
		break;

		case 1: //foward right
			motor[LDriveMotor] = 100;
			motor[RDriveMotor] = 50;
		break;

		case 2: //right
			motor[LDriveMotor] = 100;
			motor[RDriveMotor] = 0;
		break;

		case 3: //backward right
			motor[LDriveMotor] = -100;
			motor[RDriveMotor] = -50;
		break;

		case 4: //backward
			motor[LDriveMotor] = -100;
			motor[RDriveMotor] = -100;
		break;

		case 5: //backward left
			motor[LDriveMotor] = -50;
			motor[RDriveMotor] = -100;
		break;

		case 6: //left
			motor[LDriveMotor] = 0;
			motor[RDriveMotor] = 100;
		break;

		case 7: //forward left
			motor[LDriveMotor] = 50;
			motor[RDriveMotor] = 100;
		break;

	}
	return 1;

}

//Pre-teleop, post-autonomous phase, autonomous action to init robot. Must time-out in a short amount of time to pass to teleop code
void initializeRobot() {


  return;
}

task main() {
  initializeRobot();

  waitForStart();   //Stop until teleop starts

  while (1) {

  	getJoystickSettings(joystick);

  	if( !hatAction() ) //If hat is not pressed
  		drive(joystick.joy1_y1, joystick.joy1_y2); //Use joystick to drive

  	wait1Msec(10); //sleep 10ms to stabilize code execution.

  }
}
