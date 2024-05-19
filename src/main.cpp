#include <stdio.h>
#include "../include/core.h"
#include "../include/step.h"
#include <wiringPi.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
using namespace std;

const int SELECTED_MODE = QUARTER_STEP;

int main()
{
    wiringPiSetup();
    initPins();
    disableOutput();
    setDriveMode('x', SELECTED_MODE);
    setDriveMode('y', SELECTED_MODE);
    setDriverStatus('x', DRIVER_ENABLE);
    setDriverStatus('y', DRIVER_ENABLE);

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
    short isStarted = 1;

    // cout << "Auto Homing...\n";
    // autoHome();
    // // isStarted = 1;

    for (;;)
    {
        if (isStarted == 1)
        {

            while (getline(ReadFile, read))
            {
                stringstream s(read);
                cout << "read" << read << endl;
                printf("currentX: %f  currentY: %f\n", currentX, currentY);

                while (s >> part)
                {
                    cout << "part " << part << endl;

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
                        directionY = pointY < currentY ? '-' : '+'; // CHECK HERE FOR COORDINATE ROTATION
                    }
                }

                cout << "dirX: " << directionX << "dirY: " << directionY << "tarX: " << targetX << "tarY: " << targetY << endl;

                moveAxis(directionX, targetX, directionY, targetY, oneStepDistanceX, oneStepDistanceY);

                currentX = pointX;
                currentY = pointY;
                // delay(100); // CHECK HERE
            }

            printf("End of program :)\n");
            setDriverStatus('x', DRIVER_DISABLE);
            setDriverStatus('y', DRIVER_DISABLE);
            isStarted = 0;
            break;
        }
    }

    return 0;
}
