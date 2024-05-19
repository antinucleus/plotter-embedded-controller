#include <stdio.h>
#include "../include/servo.h"
#include "../include/core.h"
#include "../include/step.h"
#include <wiringPi.h>
#include <softServo.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
using namespace std;

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
    char directionX, directionY;
    short isStarted = 0;

    cout << "Auto Homing...\n";
    moveTool('+');
    autoHome();
    setDriveMode('x', SELECTED_MODE);
    setDriveMode('y', SELECTED_MODE);
    isStarted = 1;

    for (;;)
    {
        if (isStarted == 1)
        {

            while (getline(ReadFile, read))
            {
                stringstream s(read);
                // printf("currentX: %f  currentY: %f\n", currentX, currentY);

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
                        directionX = pointX < currentX ? '-' : '+';
                    }

                    if (part[0] == 'Y')
                    {
                        pointY = stod(part.substr(1));
                        targetY = abs(pointY - currentY);
                        directionY = pointY < currentY ? '+' : '-'; // CHECK HERE FOR COORDINATE ROTATION
                    }
                }

                cout << "dirX: " << directionX << "dirY: " << directionY << "tarX: " << targetX << "tarY: " << targetY << endl;

                moveAxis(directionX, targetX, directionY, targetY, oneStepDistanceX, oneStepDistanceY);

                currentX = pointX;
                currentY = pointY;
            }

            printf("End of program :)\n");
            moveTool('+');
            serveBed();
            setDriverStatus('x', DRIVER_DISABLE);
            setDriverStatus('y', DRIVER_DISABLE);
            isStarted = 0;
            break;
        }
    }

    return 0;
}
