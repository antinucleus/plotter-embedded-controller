#include <iostream>
#include <string>
#include "../include/core.h"
#include "../include/step.h"
#include "../include/servo.h"
#include "../include/api.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
using namespace std;
using namespace rapidjson;

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
    double oneStepDistance = 0;

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

void manualMode(string response, double *currentX, double *currentY, short *isStarted)
{
    double oneStepDistanceX, oneStepDistanceY;
    printf("incoming data:%s\n", response.c_str());

    Document d;
    d.Parse(response.c_str());

    Value &autoHoming = d["autoHoming"];
    Value &startPlotting = d["startPlotting"];
    Value &manualControl = d["manualControl"];
    Value &penPosition = d["penPosition"];
    Value &isMovingPen = d["isMovingPen"];
    Value &isMovingX = d["isMovingX"];
    Value &isMovingY = d["isMovingY"];
    Value &targetDistanceX = d["targetDistanceX"];
    Value &targetDistanceY = d["targetDistanceY"];
    Value &direction = d["direction"];
    Value &driveMode = d["driveMode"];

    if (autoHoming == "yes")
    {
        cout << "-----AUTO HOME STARTED----" << endl;
        autoHome(currentX, currentY);
        sendCoordinates(*currentX, *currentY);
        updateAutoHomeStatus("no");
        cout << "-----AUTO HOME FINISHED----" << endl;
    }

    if (startPlotting == "yes")
    {
        *isStarted = 1;
    }

    if (isMovingPen == "yes")
    {
        if (penPosition == "up")
        {
            moveTool('+');
        }
        else if (penPosition == "down")
        {
            moveTool('-');
        }

        updateMovingPenStatus("no");
    }

    if (isMovingX == "yes")
    {
        setDriverStatus('x', DRIVER_ENABLE);
        setDriveModeManualControl('x', driveMode.GetString());

        oneStepDistanceX = getOneStepDistanceManualControl(driveMode.GetString());
        moveAxis(direction.GetString(), targetDistanceX.GetDouble(), "-", 0, oneStepDistanceX, 0);

        *currentX += direction == "+" ? targetDistanceX.GetDouble() : -targetDistanceX.GetDouble();

        sendCoordinates(*currentX, *currentY);
        updateMovingAxisStatus('x', "no");
    }

    if (isMovingY == "yes")
    {
        setDriverStatus('y', DRIVER_ENABLE);
        setDriveModeManualControl('y', driveMode.GetString());

        oneStepDistanceY = getOneStepDistanceManualControl(driveMode.GetString());
        moveAxis("-", 0, direction.GetString(), targetDistanceY.GetDouble(), 0, oneStepDistanceY);

        *currentY += direction == "+" ? targetDistanceY.GetDouble() : -targetDistanceY.GetDouble();

        sendCoordinates(*currentX, *currentY);
        updateMovingAxisStatus('y', "no");
    }
}