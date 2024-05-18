#ifndef _STEP_H
#define _STEP_H
// axis: (x-axis = x, y-axis = y)
// direction: (r = 0, l = 1)
// oneStepDistance: one step will moveplotter by a distance of 0.16 mm in full-step mode
// targetDistance: movement distance

void moveAxis(char axis, char direction, double oneStepDistance, double targetDistance);

#endif