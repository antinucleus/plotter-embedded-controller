#include "../include/core.h"
#include <wiringPi.h>

void setDriverStatus(char axis, int status)
{
    if (axis == 'x')
    {
        digitalWrite(ENABLE_PIN_X, status);
    }
    else
    {
        digitalWrite(ENABLE_PIN_Y, status);
    }
}

void setDriveMode(int mode)
{
    if (mode == FULL_STEP)
    {
        digitalWrite(MS1_X, LOW);
        digitalWrite(MS2_X, LOW);
        digitalWrite(MS3_X, LOW);
        digitalWrite(MS1_Y, LOW);
        digitalWrite(MS2_Y, LOW);
        digitalWrite(MS3_Y, LOW);
    }
    else if (mode == HALF_STEP)
    {
        digitalWrite(MS1_X, HIGH);
        digitalWrite(MS2_X, LOW);
        digitalWrite(MS3_X, LOW);
        digitalWrite(MS1_Y, HIGH);
        digitalWrite(MS2_Y, LOW);
        digitalWrite(MS3_Y, LOW);
    }
    else if (mode == QUARTER_STEP)
    {
        digitalWrite(MS1_X, LOW);
        digitalWrite(MS2_X, HIGH);
        digitalWrite(MS3_X, LOW);
        digitalWrite(MS1_Y, LOW);
        digitalWrite(MS2_Y, HIGH);
        digitalWrite(MS3_Y, LOW);
    }
    else if (mode == EIGHTH_STEP)
    {
        digitalWrite(MS1_X, HIGH);
        digitalWrite(MS2_X, HIGH);
        digitalWrite(MS3_X, LOW);
        digitalWrite(MS1_Y, HIGH);
        digitalWrite(MS2_Y, HIGH);
        digitalWrite(MS3_Y, LOW);
    }
    else if (mode == SIXTEENTH_STEP)
    {
        digitalWrite(MS1_X, HIGH);
        digitalWrite(MS2_X, HIGH);
        digitalWrite(MS3_X, HIGH);
        digitalWrite(MS1_Y, HIGH);
        digitalWrite(MS2_Y, HIGH);
        digitalWrite(MS3_Y, HIGH);
    }
}

void initPins()
{
    pinMode(ENABLE_PIN_X, OUTPUT);
    pinMode(DIRECTION_PIN_X, OUTPUT);
    pinMode(STEP_PIN_X, OUTPUT);
    pinMode(MS1_X, OUTPUT);
    pinMode(MS2_X, OUTPUT);
    pinMode(MS3_X, OUTPUT);
    pinMode(ENABLE_PIN_Y, OUTPUT);
    pinMode(DIRECTION_PIN_Y, OUTPUT);
    pinMode(STEP_PIN_Y, OUTPUT);
    pinMode(MS1_Y, OUTPUT);
    pinMode(MS2_Y, OUTPUT);
    pinMode(MS3_Y, OUTPUT);
    pinMode(LIMIT_SWITCH_X, INPUT);
    pinMode(LIMIT_SWITCH_Y, INPUT);
}

void disableOutput()
{
    digitalWrite(ENABLE_PIN_X, DRIVER_DISABLE);
    digitalWrite(DIRECTION_PIN_X, LOW);
    digitalWrite(STEP_PIN_X, LOW);
    digitalWrite(MS1_X, LOW);
    digitalWrite(MS2_X, LOW);
    digitalWrite(MS3_X, LOW);
    digitalWrite(ENABLE_PIN_Y, DRIVER_DISABLE);
    digitalWrite(DIRECTION_PIN_Y, LOW);
    digitalWrite(STEP_PIN_Y, LOW);
    digitalWrite(MS1_Y, LOW);
    digitalWrite(MS2_Y, LOW);
    digitalWrite(MS3_Y, LOW);
}

int getStepPerRev(int mode)
{
    return ONE_CYCLE / (DEFAULT_ANGLE / mode);
}

// returning unit is mm
double getOneStepDistance(int mode)
{
    return (PULLEY_CIRCUMFERENCE / ONE_CYCLE) * (DEFAULT_ANGLE / mode);
}

int readSwitch(char axis)
{
    int readValue;
    if (axis == 'x')
    {
        readValue = digitalRead(LIMIT_SWITCH_X);
    }
    else
    {
        readValue = digitalRead(LIMIT_SWITCH_Y);
    }

    return readValue;
}