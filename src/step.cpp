#include "../include/step.h"
#include "../include/core.h"
#include <wiringPi.h>
#include <stdio.h>

// axis: (x-axis = x, y-axis = y)
// direction: (r = 0, l = 1)
// oneStepDistance: one step will moveplotter by a distance of 0.16 mm in full-step mode
// targetDistance: movement distance

void moveAxis(char axis, char direction, double oneStepDistance, double targetDistance)
{
    int stepPin, directionPin;
    double distance = 0;

    if (axis == 'x')
    {
        stepPin = STEP_PIN_X;
        directionPin = DIRECTION_PIN_X;
    }
    else
    {
        stepPin = STEP_PIN_Y;
        directionPin = DIRECTION_PIN_Y;
    }

    if (direction == 'r')
    {
        digitalWrite(directionPin, HIGH);
    }
    else
    {
        digitalWrite(directionPin, LOW);
    }

    while (distance < targetDistance)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(DELAY);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(DELAY);
        printf("distance: %f\n", distance); /// remove
        distance += oneStepDistance;
    }
}