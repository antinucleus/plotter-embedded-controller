#include "../include/step.h"
#include "../include/core.h"
#include "../include/servo.h"
#include <wiringPi.h>
#include <iostream>

void moveAxis(std::string directionX, double targetDistanceX, std::string directionY, double targetDistanceY, double oneStepDistanceX, double oneStepDistanceY)
{

    double distanceX = 0.0, distanceY = 0.0;

    if (directionX == "+")
    {
        digitalWrite(DIRECTION_PIN_X, HIGH);
    }
    else
    {
        digitalWrite(DIRECTION_PIN_X, LOW);
    }
    if (directionY == "+")
    {
        digitalWrite(DIRECTION_PIN_Y, HIGH);
    }
    else
    {
        digitalWrite(DIRECTION_PIN_Y, LOW);
    }

    while (1)
    {
        if (distanceX < targetDistanceX)
        {
            digitalWrite(STEP_PIN_X, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_X, LOW);
            delayMicroseconds(DELAY);
            distanceX += oneStepDistanceX;
        }

        if (distanceY < targetDistanceY)
        {
            digitalWrite(STEP_PIN_Y, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_Y, LOW);
            delayMicroseconds(DELAY);
            distanceY += oneStepDistanceY;
        }

        if (distanceX >= targetDistanceX && distanceY >= targetDistanceY)
        {
            std::cout << "targetDistanceX: " << targetDistanceX << "distanceX" << distanceX << std::endl;
            std::cout << "targetDistanceY: " << targetDistanceY << "distanceY" << distanceY << std::endl;
            break;
        }
    }
}

void autoHome()
{
    bool isLimitedX, isLimitedY;
    int readX, readY;
    setDriveMode('x', HALF_STEP);
    setDriveMode('y', HALF_STEP);
    digitalWrite(DIRECTION_PIN_X, LOW); // -
    digitalWrite(DIRECTION_PIN_Y, LOW); // +
    moveTool('+');

    while (1)
    {
        readX = readSwitch('x');
        readY = readSwitch('y');

        if (readX == LOW)
        {
            digitalWrite(STEP_PIN_X, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_X, LOW);
            delayMicroseconds(DELAY);
        }

        if (readY == LOW)
        {
            digitalWrite(STEP_PIN_Y, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_Y, LOW);
            delayMicroseconds(DELAY);
        }

        if (readX == HIGH && readY == HIGH)
        {
            break;
        }
    }

    moveTool('-');
}

void serveBed()
{
    bool isLimitedX, isLimitedY;
    int readX, readY;

    setDriveMode('x', HALF_STEP);
    setDriveMode('y', HALF_STEP);
    digitalWrite(DIRECTION_PIN_X, LOW);  // -
    digitalWrite(DIRECTION_PIN_Y, HIGH); // -
    moveTool('+');

    while (1)
    {
        readX = readSwitch('x');
        // readY = readSwitch('y');

        if (readX == LOW)
        {
            digitalWrite(STEP_PIN_X, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_X, LOW);
            delayMicroseconds(DELAY);
        }

        // if (readY == LOW)
        // {
        //     digitalWrite(STEP_PIN_Y, HIGH);
        //     delayMicroseconds(DELAY);
        //     digitalWrite(STEP_PIN_Y, LOW);
        //     delayMicroseconds(DELAY);
        // }

        if (readX == HIGH) // && readY == HIGH
        {
            break;
        }
    }

    moveTool('-');
}