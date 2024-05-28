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
using namespace std;

const int SELECTED_MODE = QUARTER_STEP;
const int FETCH_INTERVAL = 300; // ms

int main()
{
    wiringPiSetup();
    initPins();
    disableOutput();
    setDriverStatus('x', DRIVER_ENABLE);
    setDriverStatus('y', DRIVER_ENABLE);

    ifstream ReadFile;
    double oneStepDistanceX = getOneStepDistance(SELECTED_MODE);
    double oneStepDistanceY = getOneStepDistance(SELECTED_MODE);
    double currentX = 0.0f;
    double currentY = 0.0f;
    double pointX = 0.0f;
    double pointY = 0.0f;
    double targetX = 0.0f;
    double targetY = 0.0f;
    short isStarted = 0;
    short isStopped = 0;
    string read;
    string part;
    string directionX, directionY;
    string response;
    string url = "/machine/status";
    string fileDir = "/home/oxygen/Desktop/PlotterController/plotter-express-server/output.gcode";

    unsigned int timer = millis();

    for (;;)
    {

        if (millis() - timer > FETCH_INTERVAL)
        {
            response = getData(url);

            if (response != "")
            {
                manualMode(response, &currentX, &currentY, &isStarted);
            }
            else
            {
                printf("No response\n");
            }

            timer = millis();
        }

        if (isStarted == 1)
        {
            autoHome(&currentX, &currentY);
            setDriveMode('x', SELECTED_MODE);
            setDriveMode('y', SELECTED_MODE);
            oneStepDistanceX = getOneStepDistance(SELECTED_MODE);
            oneStepDistanceY = getOneStepDistance(SELECTED_MODE);
            setDriverStatus('x', DRIVER_ENABLE);
            setDriverStatus('y', DRIVER_ENABLE);

            ReadFile.open(fileDir);

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

                cout << "dirX: " << directionX << "\tdirY: " << directionY << "\ttarX: " << targetX << "\ttarY: " << targetY << endl;

                moveAxis(directionX, targetX, directionY, targetY, oneStepDistanceX, oneStepDistanceY);

                currentX = pointX;
                currentY = pointY;
                sendCoordinates(currentX, currentY);
                isStopped = checkStopPlotting(&isStarted);

                if (isStopped == 1)
                {
                    printf("Program aborted\n");
                    break;
                }
            }

            printf("End of program :)\n");
            serveBed();
            updateStartPlottingStatus("no");
            ReadFile.close();
            isStarted = 0;
            isStopped = 0;
        }
    }

    return 0;
}
