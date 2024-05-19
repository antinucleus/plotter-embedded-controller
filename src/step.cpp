#include "../include/step.h"
#include "../include/core.h"
#include <wiringPi.h>
#include <iostream>

void moveAxis(char directionX, double targetDistanceX, char directionY, double targetDistanceY, double oneStepDistance)
{

    double distanceX = 0, distanceY = 0;

    if (directionX == '+')
    {
        digitalWrite(DIRECTION_PIN_X, HIGH);
    }
    else
    {
        digitalWrite(DIRECTION_PIN_X, LOW);
    }
    if (directionY == '+')
    {
        digitalWrite(DIRECTION_PIN_Y, LOW);
    }
    else
    {
        digitalWrite(DIRECTION_PIN_Y, HIGH);
    }

    while (1)
    {
        if (distanceX < targetDistanceX)
        {
            digitalWrite(STEP_PIN_X, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_X, LOW);
            delayMicroseconds(DELAY);
            distanceX += oneStepDistance;
        }

        if (distanceY < targetDistanceY)
        {
            digitalWrite(STEP_PIN_Y, HIGH);
            delayMicroseconds(DELAY);
            digitalWrite(STEP_PIN_Y, LOW);
            delayMicroseconds(DELAY);
            distanceY += oneStepDistance;
        }

        if (distanceX >= targetDistanceX && distanceY >= targetDistanceY)
        {
            break;
        }
    }
}

void autoHome()
{
    bool isLimitedX,  isLimitedY;
    int readX,readY;
    digitalWrite(DIRECTION_PIN_X, LOW);  // -
    digitalWrite(DIRECTION_PIN_Y, LOW); // +


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

        if(readX == HIGH && readY == HIGH) { break;}

    }
}