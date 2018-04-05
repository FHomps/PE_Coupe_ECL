#ifndef SONAR_C
#define SONAR_C

#include "config.c"
#include "movement.c"

// Les deux fonctions suivantes permettent d'obtenir la distance (en mm) entre le sonar avant (respectivement arriere) du robot et l'obstacle detecte.

task avoidObstacles() {
	struct Config const* c = NULL;
	getConfig(&c);

	while (true) {
		wait1Msec(20);
#ifdef sensorFront
		displayBigTextLine(7, "1");
		bool obstacleFront = SensorValue[sensorFront] * 10 < c->securityDistance;
#else
		displayBigTextLine(9, "2");
		bool obstacleFront = false;
#endif
#ifdef sensorBack
		bool obstacleBack = SensorValue[sensorBack] * 10 < c->securityDistance;
#else
		bool obstacleBack = false;
#endif
		if ((getMovementType() == MOVETO && obstacleFront)
				|| (getMovementType() == MOVETO_BACKWARDS && obstacleBack))
			{ pauseMovement(); }
		else if ((!obstacleFront && !obstacleBack)
			  || (getMovementType() == MOVETO && !obstacleFront)
				|| (getMovementType() == MOVETO_BACKWARDS && !obstacleBack))
			{ resumeMovement(); }
	}
}

#endif