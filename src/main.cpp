#include <stdio.h>
#include "../include/servo.h"
#include "../include/core.h"
#include "../include/step.h"
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
    int timeLimit = 1000; // millis
    short isStarted = 0;
    string get;
    unsigned int timer = millis();

    for (;;)
    {
        if (millis() - timer > timeLimit)
        {
            get = getData("/machine/status");

            if (get != "")
            {
                printf("incoming data:%s\n", get.c_str());

                Document d;
                d.Parse(get.c_str());

                Value &startPlotting = d["startPlotting"];
                Value &penDirection = d["penDirection"];

                cout << "START PLOTTING: " << startPlotting.GetInt() << endl;
                cout << "PEN DIR: " << penDirection.GetString() << endl;
                movePen("none");
            }
            else
            {
                printf("no data\n");
            }

            timer = millis();
        }

        if (isStarted == 1)
        {
            while (getline(ReadFile, read))
            {
                stringstream s(read);

                while (s >> part)
                {
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

                currentX = pointX;
                currentY = pointY;

                sendCoordinates(currentX, currentY);
            }
        }
    }

    // wiringPiSetup();
    // initPins();
    // disableOutput();
    // setDriverStatus('x', DRIVER_ENABLE);
    // setDriverStatus('y', DRIVER_ENABLE);
    // softServoSetup(23, 23, 23, 23, 23, 23, 23, 23);

    // double oneStepDistanceX = getOneStepDistance(SELECTED_MODE);
    // double oneStepDistanceY = getOneStepDistance(SELECTED_MODE);
    // string read;
    // string part;
    // ifstream ReadFile("test.gcode");
    // double currentX = 0.0f;
    // double currentY = 0.0f;
    // double pointX = 0.0f;
    // double pointY = 0.0f;
    // double targetX = 0.0f;
    // double targetY = 0.0f;
    // char directionX, directionY;
    // short isStarted = 0;

    // cout << "Auto Homing...\n";
    // moveTool('+');
    // autoHome();
    // setDriveMode('x', SELECTED_MODE);
    // setDriveMode('y', SELECTED_MODE);
    // isStarted = 1;

    // for (;;)
    // {
    //     if (isStarted == 1)
    //     {

    //         while (getline(ReadFile, read))
    //         {
    //             stringstream s(read);
    //             // printf("currentX: %f  currentY: %f\n", currentX, currentY);

    //             while (s >> part)
    //             {
    //                 if (part == "G0")
    //                 {
    //                     moveTool('+');
    //                 }

    //                 if (part == "G1")
    //                 {
    //                     moveTool('-');
    //                 }

    //                 if (part[0] == 'X')
    //                 {
    //                     pointX = stod(part.substr(1));
    //                     targetX = abs(pointX - currentX);
    //                     directionX = pointX < currentX ? '-' : '+';
    //                 }

    //                 if (part[0] == 'Y')
    //                 {
    //                     pointY = stod(part.substr(1));
    //                     targetY = abs(pointY - currentY);
    //                     directionY = pointY < currentY ? '+' : '-'; // CHECK HERE FOR COORDINATE ROTATION
    //                 }
    //             }

    //             cout << "dirX: " << directionX << "dirY: " << directionY << "tarX: " << targetX << "tarY: " << targetY << endl;

    //             moveAxis(directionX, targetX, directionY, targetY, oneStepDistanceX, oneStepDistanceY);

    //             currentX = pointX;
    //             currentY = pointY;
    //             cout << "Response: " << postData(currentX, currentY) << endl;

    //         }

    //         printf("End of program :)\n");
    //         moveTool('+');
    //         serveBed();
    //         moveTool('-');
    //         setDriverStatus('x', DRIVER_DISABLE);
    //         setDriverStatus('y', DRIVER_DISABLE);
    //         isStarted = 0;
    //         break;
    //     }
    // }

    return 0;
}
