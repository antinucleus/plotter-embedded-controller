#ifndef _STEP_H
#define _STEP_H

// directionX, directionY: -, +
// targetDistanceX, targetDistanceY: desired distances in mm
// oneStepDistance: one step will move plotter by a distance of 0.16 mm in full-step mode

void moveAxis(char directionX, double targetDistanceX, char directionY, double targetDistanceY, double oneStepDistanceX, double oneStepDistanceY);
void autoHome();

#endif