#pragma config(Hubs,  S1, MatrxRbtcs, none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          alternateur,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_Matrix_S1_1, motorRight,    tmotorMatrix, openLoop)
#pragma config(Motor,  mtr_Matrix_S1_2, motorE,        tmotorMatrix, openLoop)
#pragma config(Motor,  mtr_Matrix_S1_3, motorLeft,     tmotorMatrix, openLoop)
#pragma config(Motor,  mtr_Matrix_S1_4, catapulte,     tmotorMatrix, openLoop)
#pragma config(Servo,  srvo_Matrix_S1_1, servo1,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_2, servo2,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_3, servo3,               tServoNone)
#pragma config(Servo,  srvo_Matrix_S1_4, servo4,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void giveOneBall() {
	motor[alternateur]=40; wait1Msec(150);
	motor[alternateur]=0;	wait1Msec(100);
	motor[alternateur]=-40;	wait1Msec(150);
	motor[alternateur]=0;
}
void throwOneBall() {
	motor[catapulte]=100; wait1Msec(60);
	motor[catapulte]=0;	wait1Msec(500);
	motor[catapulte]=-15;	wait1Msec(200);
	motor[catapulte]=0;
}

task main()
{
	for (int j=0;j<6;j++) {
		giveOneBall();
		wait1Msec(2000);
		throwOneBall();
		wait1Msec(2000);

	}
}
