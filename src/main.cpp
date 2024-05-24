#include <stdio.h>
#include "../include/core.h"
#include "../include/step.h"
#include "../include/servo.h"
#include "../include/utils.h"
#include "../include/api.h"
#include <wiringPi.h>
#include <softServo.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <curl/curl.h>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
using namespace std;
using namespace rapidjson;

const int SELECTED_MODE = SIXTEENTH_STEP;

int main()
{
    wiringPiSetup();
    initPins();
    disableOutput();
    setDriverStatus('x', DRIVER_ENABLE);
    setDriverStatus('y', DRIVER_ENABLE);
    softServoSetup(23, 23, 23, 23, 23, 23, 23, 23);

    double oneStepDistanceX = getOneStepDistance(SELECTED_MODE);
    double oneStepDistanceY = getOneStepDistance(SELECTED_MODE);
    string read;
    string part;
    ifstream ReadFile("test.gcode");
    double currentX = 0.0f;
    double currentY = 0.0f;
    double pointX = 0.0f;
    double pointY = 0.0f;
    double targetX = 0.0f;
    double targetY = 0.0f;
    string directionX, directionY;
    short isStarted = 0;
    int fetchInterval = 1000; // millis
    string response;
    unsigned int timer = millis();

    for (;;)
    {

        if (millis() - timer > fetchInterval)
        {
            response = getData("/machine/status");

            if (response != "")
            {
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
                    autoHome();
                    cout << "-----AUTO HOME FINISHED----" << endl;
                    updateAutoHomeStatus("no");
                }

                if (startPlotting == "yes")
                {
                    isStarted = 1;
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
                    setDriverStatus('y', DRIVER_ENABLE);

                    setDriveModeManualControl('x', driveMode.GetString());

                    oneStepDistanceX = getOneStepDistanceManualControl(driveMode.GetString());
                    moveAxis(direction.GetString(), targetDistanceX.GetDouble(), "-", 0, oneStepDistanceX, 0);
                    if (direction == "+")
                    {
                        currentX += targetDistanceX.GetDouble();
                    }
                    else
                    {
                        currentX -= targetDistanceX.GetDouble();
                    }

                    sendCoordinates(currentX, currentY);

                    updateMovingAxisStatus('x', "no");

                    setDriverStatus('x', DRIVER_DISABLE);
                    setDriverStatus('y', DRIVER_DISABLE);
                }

                if (isMovingY == "yes")
                {
                    setDriverStatus('x', DRIVER_ENABLE);
                    setDriverStatus('y', DRIVER_ENABLE);

                    setDriveModeManualControl('y', driveMode.GetString());

                    oneStepDistanceY = getOneStepDistanceManualControl(driveMode.GetString());
                    moveAxis("-", 0, direction.GetString(), targetDistanceY.GetDouble(), 0, oneStepDistanceY);

                    if (direction == "+")
                    {
                        currentY += targetDistanceY.GetDouble();
                    }
                    else
                    {
                        currentY -= targetDistanceY.GetDouble();
                    }

                    sendCoordinates(currentX, currentY);

                    updateMovingAxisStatus('y', "no");

                    setDriverStatus('x', DRIVER_DISABLE);
                    setDriverStatus('y', DRIVER_DISABLE);
                }
            }
            else
            {
                printf("No response\n");
            }

            timer = millis();
        }

        if (isStarted == 1)
        {
            setDriverStatus('x', DRIVER_ENABLE);
            setDriverStatus('y', DRIVER_ENABLE);
            oneStepDistanceX = getOneStepDistance(SELECTED_MODE);
            oneStepDistanceY = getOneStepDistance(SELECTED_MODE);
            setDriveMode('x', SELECTED_MODE);
            setDriveMode('y', SELECTED_MODE);
            autoHome();
            currentX = 0.0;
            currentY = 0.0;

            while (getline(ReadFile, read))
            {
                stringstream s(read);

                while (s >> part)
                {
                    if (part == "G0")
                    {
                        moveTool('+');
                    }

                    if (part == "G1")
                    {
                        moveTool('-');
                    }

                    if (part[0] == 'X')
                    {
                        pointX = stod(part.substr(1));
                        targetX = abs(pointX - currentX);
                        directionX = pointX < currentX ? "-" : "+";
                    }

                    if (part[0] == 'Y')
                    {
                        pointY = stod(part.substr(1));
                        targetY = abs(pointY - currentY);
                        directionY = pointY < currentY ? "-" : "+"; // CHECK HERE FOR COORDINATE ROTATION
                    }
                }

                cout << "dirX: " << directionX << "dirY: " << directionY << "tarX: " << targetX << "tarY: " << targetY << endl;

                moveAxis(directionX, targetX, directionY, targetY, oneStepDistanceX, oneStepDistanceY);

                currentX = pointX;
                currentY = pointY;
                sendCoordinates(currentX, currentY);
            }

            printf("End of program :)\n");
            serveBed();
            setDriverStatus('x', DRIVER_DISABLE);
            setDriverStatus('y', DRIVER_DISABLE);
            updateStartPlottingStatus("no");
            isStarted = 0;
        }
    }

    return 0;
}
