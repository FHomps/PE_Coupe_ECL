#ifndef MOVEMENT
#define MOVEMENT

#include "config.c"
#include "position.c"

void moveTo(float x, float y); //x et y en mm, positions absolues (cible)

void moveTo_backwards(float x, float y);

void rotateTo(float orientation); //orientation en degres

void pauseMovement();

void resumeMovement();

void abortMovement();

enum movementState {NOMVT, STARTING, MOVETO, MOVETO_BACKWARDS, ROTATETO, PAUSED, FAILED_TIMEOUT, ABORTED};
enum movementState getMovementState();


//-- DEBUT DES DEFINITIONS --


enum movementState __mvtState = NOMVT;

enum movementState getMovementState() { return __mvtState; }

void pauseMovement() { __mvtState = PAUSED; }

void resumeMovement() { __mvtState = STARTING; }

void abortMovement() { __mvtState = ABORTED; }

float limit(float number, float limit) {
	if (number > limit)
		return limit;
	if (number < -limit)
		return -limit;
	return number;
}

float limit(float number, float lowerLimit, float upperLimit) {
	if (number > upperLimit)
		return upperLimit;
	if (number < lowerLimit)
		return lowerLimit;
	return number;
}

float mod2Pi(float number) {
	while (number > PI)
		number -= 2 * PI;
	while (number < -PI)
		number += 2 * PI;
	return number;
}

float modPi(float number) {
	while (number > PI / 2)
		number -= PI;
	while (number < -PI / 2)
		number += PI;
	return number;
}

float __targetX = 0; //mm
float __targetY = 0; //mm
bool __backwards = false;

task task_moveTo() {
	struct Config const* c = NULL;
	getConfig(&c);
	struct PosData pos;
	getRawPosition(&pos);
	pos.x *= c->mmPerEncode;
	pos.y *= c->mmPerEncode;

	//Pre-initialisation des variables

	float D = sqrt(pow(__targetX - pos.x, 2) + pow(__targetY - pos.y, 2)); //Distance a la cible
	float A = PI - pos.orientation; //Difference d'orientation avec la cible
	if (__targetX - pos.x + D != 0)
		A = mod2Pi(2 * atan((__targetY - pos.y) / (__targetX - pos.x + D)) - pos.orientation);

	float D_proj = cos(A) * D; //Distance signee
	float A_modPi = modPi(A); //Angle tolerant la cible vers l'arriere
	float i_D_proj = 0; //Integrale de D_proj

	float VRot = 0, VStr = 0; //Vitesses de consigne de rotation et d'avance en ligne droite

	bool finished = false;
	unsigned int counter_stop = 0;
	unsigned int counter_timeout = 0;

	do {
		wait1Msec(c->controlPeriod);

		counter_timeout += 1;
		if (__mvtState == STARTING)
			__mvtState = __backwards ? MOVETO_BACKWARDS : MOVETO;
		if (counter_timeout * c->controlPeriod > 15000)
			__mvtState = FAILED_TIMEOUT;

		getRawPosition(&pos);
		pos.x *= c->mmPerEncode;
		pos.y *= c->mmPerEncode;

		//Integration en deux parties (transformee en z) : ancien + nouveau / 2
		i_D_proj += D_proj / 2 * c->controlPeriod;

		D = sqrt(pow(__targetX - pos.x, 2) + pow(__targetY - pos.y, 2));
		A = PI - pos.orientation;
		if (__targetX - pos.x + D != 0)
			A = mod2Pi(2 * atan((__targetY - pos.y) / (__targetX - pos.x + D)) - pos.orientation);

		D_proj = cos(A) * D;
		A_modPi = modPi(A);

		//Deuxieme partie de l'integration
		i_D_proj += D_proj / 2 * c->controlPeriod;

		float dVRot = 0, dVStr = 0;
		//Asservissement en 3 phases : on ne lance les integrations que lorsque la valeur est suffisamment proche de la cible

		if (D <= c->dist_closeEnough || __mvtState == PAUSED || __mvtState == ABORTED || __mvtState == FAILED_TIMEOUT) { //Phase d'arret progressif
			dVRot = -VRot;
			dVStr = -VStr;
			if (counter_stop * c->controlPeriod > c->maxSpeed / c->maxAccel + 100 && __mvtState != PAUSED) //On n'arrete pas le mouvement en cas de pause
				__mvtState = NOMVT;
			counter_stop += 1;
			counter_timeout = 0;
		}
		else if (D > c->dist_close) {
			if (!__backwards) {
				if (fabs(A) > PI / 24) { //Phase 1 : Rotation simple
					dVRot = c->KPRot * A - VRot;
	        dVStr = -VStr;
	        i_D_proj = 0;
				}
				else { //Phase 2 : rotation simple, avance simple
					dVRot = c->KPRot * A_modPi - VRot;
	        dVStr = c->KPStr * D_proj - VStr;
	        i_D_proj = 0;
				}
			}
			else {
				float A_back = mod2Pi(A + PI);
				if (fabs(A_back) > PI / 24) {
					dVRot = c->KPRot * A_back - VRot;
	        dVStr = -VStr;
	        i_D_proj = 0;
				}
				else {
					dVRot = c->KPRot * A_modPi - VRot;
	        dVStr = c->KPStr * D_proj - VStr;
	        i_D_proj = 0;
				}
			}
			counter_stop = 0;
		}
		else if (D > c->dist_closeEnough) { //Phase 3 : rotation simple, avance integree
			dVRot = c->KPRot * A_modPi - VRot;
      dVStr = c->KPStr * D_proj + c->KIStr * i_D_proj - VStr;
      displayBigTextLine(1, "%5.1f", i_D_proj);
      counter_stop = 0;
		}

		//Limitation des dV a l'acceleration maximale
		dVRot = limit(dVRot, c->maxAccel * 0.5 * c->controlPeriod / c->mmPerEncode * c->powerPerEPms);
    dVStr = limit(dVStr, c->maxAccel * c->controlPeriod / c->mmPerEncode * c->powerPerEPms - abs(dVRot));

    //Limitation des V a la vitesse maximale
    VRot = limit(VRot + dVRot, c->maxSpeed * 0.5 * c->powerPerEPms / c->mmPerEncode);
    VStr = limit(VStr + dVStr, c->maxSpeed * c->powerPerEPms / c->mmPerEncode - abs(VRot));

    motor[motorRight] = VStr + VRot;
    motor[motorLeft] = VStr - VRot;

	} while (__mvtState != NOMVT);
}

void moveTo(float x, float y) {
	while (__mvtState != NOMVT) { wait1Msec(10); }
	wait1Msec(20); //IMPORTANT pour laisser au mouvement precedent le temps de se finir correctement

	struct Config const* c = NULL;
	getConfig(&c);
	__targetX = x;
	__targetY = y;
	__backwards = false;
	__mvtState = STARTING;
	startTask(task_moveTo);
}

void moveTo_backwards(float x, float y) {
	while (__mvtState != NOMVT) { wait1Msec(10); }
	wait1Msec(20);

	struct Config const* c = NULL;
	getConfig(&c);
	__targetX = x;
	__targetY = y;
	__backwards = true;
	__mvtState = STARTING;
	startTask(task_moveTo);
}

float __targetOrientation; //rad

task task_rotateTo() {
	struct Config const* c = NULL;
	getConfig(&c);
	struct PosData pos;
	getRawPosition(&pos);
	pos.x *= c->mmPerEncode;
	pos.y *= c->mmPerEncode;

	float VRot = 0, VStr = 0;

	float oriDiff = mod2Pi(__targetOrientation - pos.orientation);
	float i_oriDiff = 0;

	float D = c->mmPerEncode * sqrt(pow(__targetX - pos.x, 2) + pow(__targetY - pos.y, 2));

	float A = PI - pos.orientation; //Argument cible - robot
	if (__targetX - pos.x + D != 0)
		A = mod2Pi(2 * atan((__targetY - pos.y) / (__targetX - pos.x + D)) - pos.orientation);

	unsigned int counter_stop = 0;
	unsigned int counter_timeout = 0;

	do {
		wait1Msec(c->controlPeriod);

		counter_timeout += 1;
		if (__mvtState == STARTING)
			__mvtState = ROTATETO;
		if (counter_timeout * c->controlPeriod > 15000)
			__mvtState = FAILED_TIMEOUT;

		getRawPosition(&pos);
		pos.x *= c->mmPerEncode;
		pos.y *= c->mmPerEncode;

		i_oriDiff += oriDiff / 2 * c->controlPeriod;
		oriDiff = mod2Pi(__targetOrientation - pos.orientation);
		i_oriDiff += oriDiff / 2 * c->controlPeriod;

		A = PI - pos.orientation; //Argument cible - robot
		if (__targetX - pos.x + D != 0)
			A = mod2Pi(2 * atan((__targetY - pos.y) / (__targetX - pos.x + D)) - pos.orientation);
		D = c->mmPerEncode * sqrt(pow(__targetX - pos.x, 2) + pow(__targetY - pos.y, 2));

		float dVRot, dVStr;

		if (fabs(oriDiff) <= c->dist_closeEnough / c->betweenWheels || __mvtState == PAUSED || __mvtState == ABORTED || __mvtState == FAILED_TIMEOUT){
			dVRot = -VRot;
			dVStr = -VStr;
			i_oriDiff = 0;
			counter_stop += 1;
			counter_timeout = 0;

			if ((counter_stop * c->controlPeriod > c->maxSpeed / c->maxAccel + 100) && __mvtState != PAUSED)
				__mvtState = NOMVT;
		}
		else if (fabs(oriDiff) > c->dist_close / c->betweenWheels) {
			dVRot = c->KPRot * oriDiff - VRot;
			dVStr = c->KPStr * D * cos(A) - VStr;
			i_oriDiff = 0;
			counter_stop = 0;
		}
		else if (fabs(oriDiff) > c->dist_closeEnough / c->betweenWheels) {
			displayBigTextLine(7, "%5.1f", i_oriDiff);
			dVRot = c->KPRot * oriDiff + c->KIRot * i_oriDiff - VRot;
			dVStr = c->KPStr * D * cos(A) - VStr;
			counter_stop = 0;
		}

		//Limitation des dV a l'acceleration maximale
		dVRot = limit(dVRot, c->maxAccel * 0.6 * c->controlPeriod / c->mmPerEncode * c->powerPerEPms);
    dVStr = limit(dVStr, c->maxAccel * c->controlPeriod / c->mmPerEncode * c->powerPerEPms - abs(dVRot));

    //Limitation des V a la vitesse maximale
    VRot = limit(VRot + dVRot, c->maxSpeed * 0.6 * c->powerPerEPms / c->mmPerEncode);
    VStr = limit(VStr + dVStr, c->maxSpeed * c->powerPerEPms / c->mmPerEncode - abs(VRot));

    motor[motorRight] = VStr + VRot;
    motor[motorLeft] = VStr - VRot;

	} while (__mvtState != NOMVT);
}

void rotateTo(float orientation) {
	while (__mvtState != NOMVT) { wait1Msec(10); }
	wait1Msec(20);

	struct Config const* c = NULL;
	struct PosData pos;
	getConfig(&c);
	getRawPosition(&pos);
	__targetX = pos.x * c->mmPerEncode;
	__targetY = pos.y * c->mmPerEncode;
	__targetOrientation = orientation * PI / 180;
	__mvtState = STARTING;
	startTask(task_rotateTo);
}


#endif
