#ifndef _STEP_H
#define _STEP_H
#include <string>

// directionX, directionY: -, +
// targetDistanceX, targetDistanceY: desired distances in mm
// oneStepDistance: one step will move plotter by a distance of 0.16 mm in full-step mode

void pulseStepMotor(char axis);
void moveAxis(std::string directionX, double targetDistanceX, std::string directionY, double targetDistanceY, double oneStepDistanceX, double oneStepDistanceY);
void stayMinimumDistanceFromSwitch();
void autoHome(double *currentX, double *currentY);
void serveBed();

#endif