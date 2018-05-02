#pragma config(Sensor, S4,     redButton,      sensorEV3_Touch)
#pragma config(Motor,  motorA,          motorRight,    tmotorEV3_Medium, PIDControl, encoder)
#pragma config(Motor,  motorB,          motorLeft,     tmotorEV3_Medium, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define sensorFront S2
#define sensorBack S1

#include "config.c"
#include "position.c"
#include "movement.c"
#include "sonar.c"

float notes[8] = {1046.50, 1174.66, 1318.51, 1396.91, 1567.98, 1760.00, 1975.53, 2093.00};
unsigned int currentNote = 0;

void beep() {
	playTone(notes[currentNote], 10);
	currentNote += 1;
	if (currentNote == 8)
		currentNote = 0;
}

void fanfare() {
	playTone(notes[7], 15); wait1Msec(150);
	playTone(notes[7], 15); wait1Msec(150);
	playTone(notes[7], 15); wait1Msec(150);
	playTone(notes[7], 45); wait1Msec(450);
	playTone(1661.22, 45); wait1Msec(450);
	playTone(1864.66, 45); wait1Msec(450);
	playTone(notes[7], 15); wait1Msec(150);
	wait1Msec(150);
	playTone(1864.66, 15); wait1Msec(150);
	playTone(notes[7], 90); wait1Msec(900);
}

task emergencyStop() {
	clearTimer(T4);
	while (SensorValue[redButton] == 0 && time1[T4] < 99000) { wait1Msec(20); }
	stopAllTasks();
}

task displayPos() {
	struct PosData pos;
	while (true) {
		getPosition(&pos);
		displayBigTextLine(1, "X : %6.2f", pos.x);
		displayBigTextLine(3, "Y : %6.2f", pos.y);
		displayBigTextLine(5, "O : %6.2f", pos.orientation);
		wait1Msec(50);
	}
}

task main() {
  initConfig(TULLIUS);
  initPosition(true); //Vert
  setSoundVolume(30);

  while (SensorValue[redButton] == 1) { wait1Msec(20); }
  wait1Msec(100);
  startTask(emergencyStop);
  startTask(displayPos);
  startTask(avoidObstacles);

  PosData pos;
  struct Config const *c = NULL;

  moveTo(100, 0);
	moveTo(660, -400);
	moveTo(660, 400);
	moveTo_backwards(660, -700);
	moveTo_backwards(300, -1160);
	moveTo_backwards(300, -1450);
	while (getMovementState() != NOMVT) {wait1Msec(20);}

	getConfig(&c);
	__position.y = -1500 / c->mmPerEncode;
	__position.x = 300 / c->mmPerEncode;
	__position.orientation = PI / 2;

	moveTo(300, -1300);
	moveTo_backwards(-100, -1300);
	while (getMovementState() != NOMVT) {wait1Msec(20);}

	getConfig(&c);
	__position.y = -1300 / c->mmPerEncode;
	__position.x = 0 / c->mmPerEncode;
	__position.orientation = 0;


	moveTo(550, -1300);
	moveTo_backwards(1125, -1100);
	moveTo(825, -1200);
	moveTo_backwards(1400, -1200);
	moveTo(400, -100);
	moveTo(600, 600);
	while (getMovementState() != NOMVT) {wait1Msec(20);}
	__position.orientation = PI/2;

	moveTo_backwards(600, 400);
	moveTo_backwards(1100, 200);
	while (getMovementState() != NOMVT) {wait1Msec(20);}

	//Enlever détection d'obstacles
	doAvoidObstacles = false;
	moveTo_backwards(1100, 800);

	/*moveTo(-100, 0);
	moveTo(-500, -500);
	moveTo(-2000, -750);
	moveTo(-2000, -1200);
	moveTo(-1800, -1200);
	moveTo(-900, 300);*/


	while (getMovementState() != NOMVT) {wait1Msec(20);}
	fanfare();
}
