#include <iostream>
#include "../include/core.h"
#include "../include/step.h"

void setDriveModeManualControl(char axis, std::string driveMode)
{
    if (driveMode == "FullStep")
    {
        setDriveMode(axis, FULL_STEP);
    }
    else if (driveMode == "HalfStep")
    {
        setDriveMode(axis, HALF_STEP);
    }
    else if (driveMode == "QuarterStep")
    {
        setDriveMode(axis, QUARTER_STEP);
    }
    else if (driveMode == "EighthStep")
    {
        setDriveMode(axis, EIGHTH_STEP);
    }
    else if (driveMode == "SixteenthStep")
    {
        setDriveMode(axis, SIXTEENTH_STEP);
    }
}

double getOneStepDistanceManualControl(std::string driveMode)
{
    double oneStepDistance;

    if (driveMode == "FullStep")
    {
        oneStepDistance = getOneStepDistance(FULL_STEP);
    }
    else if (driveMode == "HalfStep")
    {
        oneStepDistance = getOneStepDistance(HALF_STEP);
    }
    else if (driveMode == "QuarterStep")
    {
        oneStepDistance = getOneStepDistance(QUARTER_STEP);
    }
    else if (driveMode == "EighthStep")
    {
        oneStepDistance = getOneStepDistance(EIGHTH_STEP);
    }
    else if (driveMode == "SixteenthStep")
    {
        oneStepDistance = getOneStepDistance(SIXTEENTH_STEP);
    }

    return oneStepDistance;
}