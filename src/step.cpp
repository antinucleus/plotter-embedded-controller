#include "../include/step.h"
#include "../include/core.h"
#include "../include/servo.h"
#include <wiringPi.h>
#include <iostream>

void pulseStepMotor(char axis)
{
    if (axis == 'x')
    {
        digitalWrite(STEP_PIN_X, HIGH);
        delayMicroseconds(DELAY);
        digitalWrite(STEP_PIN_X, LOW);
        delayMicroseconds(DELAY);
    }
    else
    {
        digitalWrite(STEP_PIN_Y, HIGH);
        delayMicroseconds(DELAY);
        digitalWrite(STEP_PIN_Y, LOW);
        delayMicroseconds(DELAY);
    }
}

void moveAxis(std::string directionX, double targetDistanceX, std::string directionY, double targetDistanceY, double oneStepDistanceX, double oneStepDistanceY)
{

    int readX, readY;
    double distanceX = 0.0f, distanceY = 0.0f;

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
        readX = readSwitch('x');
        readY = readSwitch('y');

        if (distanceX < targetDistanceX)
        {
            pulseStepMotor('x');
            distanceX += oneStepDistanceX;
        }

        if (distanceY < targetDistanceY)
        {
            pulseStepMotor('y');
            distanceY += oneStepDistanceY;
        }

        if (distanceX >= targetDistanceX && distanceY >= targetDistanceY)
        {
            break;
        }
    }
}

void stayMinimumDistanceFromSwitch()
{
    digitalWrite(DIRECTION_PIN_X, HIGH); // +
    digitalWrite(DIRECTION_PIN_Y, HIGH); // +
    int readX, readY;

    while (1)
    {
        readX = readSwitch('x');
        readY = readSwitch('y');

        if (readX == HIGH)
        {
            pulseStepMotor('x');
        }

        if (readY == HIGH)
        {
            pulseStepMotor('y');
        }

        if (readX == LOW && readY == LOW)
        {
            break;
        }
    }
}

void autoHome(double *currentX, double *currentY)
{
    int readX, readY;
    setDriveMode('x', QUARTER_STEP);
    setDriveMode('y', QUARTER_STEP);
    setDriverStatus('x', DRIVER_ENABLE);
    setDriverStatus('y', DRIVER_ENABLE);
    digitalWrite(DIRECTION_PIN_X, LOW); // -
    digitalWrite(DIRECTION_PIN_Y, LOW); // -
    moveTool('+');

    while (1)
    {
        readX = readSwitch('x');
        readY = readSwitch('y');

        if (readX == LOW)
        {
            pulseStepMotor('x');
        }

        if (readX == HIGH && readY == LOW)
        {
            pulseStepMotor('y');
        }

        if (readX == HIGH && readY == HIGH)
        {
            stayMinimumDistanceFromSwitch();
            break;
        }
    }

    *currentX = 0.0;
    *currentY = 0.0;

    moveTool('-');
    setDriverStatus('x', DRIVER_DISABLE);
    setDriverStatus('y', DRIVER_DISABLE);
}

void serveBed()
{
    int readX, readY;

    setDriveMode('x', QUARTER_STEP);
    setDriveMode('y', QUARTER_STEP);
    setDriverStatus('x', DRIVER_ENABLE);
    setDriverStatus('y', DRIVER_ENABLE);
    digitalWrite(DIRECTION_PIN_X, LOW);  // -
    digitalWrite(DIRECTION_PIN_Y, HIGH); // -
    moveTool('+');

    while (1)
    {
        readX = readSwitch('x');
        // readY = readSwitch('y');

        if (readX == LOW)
        {
            pulseStepMotor('x');
        }

        // if (readY == LOW)
        // {
        //     pulseStepMotor('y');
        // }

        if (readX == HIGH) // && readY == HIGH
        {
            break;
        }
    }

    moveTool('-');
    setDriverStatus('x', DRIVER_DISABLE);
    setDriverStatus('y', DRIVER_DISABLE);
}