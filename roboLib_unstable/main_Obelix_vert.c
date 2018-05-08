#pragma config(Hubs,  S1, MatrxRbtcs, none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorSONAR)
#pragma config(Sensor, S3,     redButton,      sensorTouch)
#pragma config(Sensor, S4,     ,               sensorSONAR)
#pragma config(Motor,  motorA,          alternateur,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_Matrix_S1_1, motorLeft,     tmotorMatrix, openLoop)
#pragma config(Motor,  mtr_Matrix_S1_2, motorE,        tmotorMatrix, openLoop)
#pragma config(Motor,  mtr_Matrix_S1_3, motorRight,    tmotorMatrix, openLoop)
#pragma config(Motor,  mtr_Matrix_S1_4, catapulte,     tmotorMatrix, openLoop)
#pragma config(Servo,  srvo_Matrix_S1_1, servo1,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_2, servo2,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_3, servo3,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_4, servo4,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define sensorFront S2
#define sensorBack S4

#include "config.c"
#include "position.c"
#include "movement.c"
#include "sonar.c"

task displayPos() {
	struct PosData pos;
	while (true) {
		wait1Msec(200);
		getPosition(&pos);
		displayBigTextLine(1, "X %5.1f", pos.x);
		displayBigTextLine(3, "Y %5.1f", pos.y);
		displayBigTextLine(5, "O %5.1f", pos.orientation);
		//writeDebugStreamLine("X:%5.1f Y:%5.1f O:%5.1f", pos.x, pos.y, pos.orientation);
	}
}

int thrownBalls = 0;

void wiggle() {
	motor[motorLeft] = 50;
	motor[motorRight] = 50;
	wait1Msec(50);
	motor[motorLeft] = -50;
	motor[motorRight] = -50;
	wait1Msec(50);
	motor[motorLeft] = 0;
	motor[motorRight] = 0;
}

void throw() {
	motor[alternateur]=40; wait1Msec(150);
	motor[alternateur]=0;	wait1Msec(100);
	motor[alternateur]=-40;	wait1Msec(150);
	motor[alternateur]=0; wait1Msec(400);
	motor[catapulte]=200; wait1Msec(200);
	motor[catapulte]=0;	wait1Msec(100);
	motor[catapulte]=-20;	wait1Msec(200);
	motor[catapulte]=0;
	wiggle();
}

task emergencyStop() {
	clearTimer(T3);
	while (SensorValue[redButton] != 1 && time1[T3] < 99000) { wait1Msec(20); }
	stopAllTasks();
}

task main()
{
  initConfig(OBELIX);
  initPosition(true);
  while (SensorValue[redButton] == 1) { wait1Msec(20); }
  wait1Msec(500);
 	startTask(displayPos);
  startTask(avoidObstacles);
  startTask(emergencyStop);

  __position.orientation = 0;
  moveTo(200, -300);
  rotateTo(0);
  moveTo(200, 300);
  rotateTo(0);
  moveTo(-200, -300);
  rotateTo(0);
  moveTo(-200, 300);
  rotateTo(0);
  moveTo(0, 0);
  rotateTo(0);

  while (true) {}

  wait1Msec(7000);

 	moveTo_backward(0, -200);
 	moveTo_backward(150, -620);
 	moveTo_backward(50, -900);
 	moveTo_forward(50, -720);
 	moveTo_forward(50, -650);
 	__config.maxSpeed = 0.3;

 	for (int i = 0; i < 8; i++) {
 		moveTo_backward(50, -740);
 		wait1Msec(200);
 		if (didTimeout)
 			break;
 		moveTo_forward(50, -650);
 		wait1Msec(200);
 		if (didTimeout)
 			break;
 	}
 	__config.maxSpeed = 0.5;

 	moveTo_forward(50, -400);
 	moveTo(100, -380);
 	rotateTo(110);
 	while (getMovementState() != NOMVT) { wait1Msec(20); }
 	for (int i = 0; i < 8; i++) {
 		throw();
 		wait1Msec(500);
 	}
 	moveTo(-50, -600);


  while (getMovementState() != NOMVT) { wait1Msec(20); }
}
