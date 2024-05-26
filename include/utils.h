#ifndef _UTILS_H
#define _UTILS_H
#include <string>

void setDriveModeManualControl(char axis, std::string driveMode);
double getOneStepDistanceManualControl(std::string driveMode);
void manualMode(std::string response, double *currentX, double *currentY, short *isStarted);
short checkStopPlotting(short *isStarted);
#endif
