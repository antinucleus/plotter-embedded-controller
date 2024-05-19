#include "../include/servo.h"
#include <softServo.h>
#include <wiringPi.h>

void moveTool(char direction)
{
    if (direction == '+')
    {
        softServoWrite(23, 400);
    }

    if (direction == '-')
    {
        softServoWrite(23, -100);
    }
}