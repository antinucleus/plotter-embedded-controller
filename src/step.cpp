#include "../include/step.h"
#include "../include/core.h"
#include <wiringPi.h>
#include <iostream>

void moveAxis(char directionX, double targetDistanceX, char directionY, double targetDistanceY, double oneStepDistanceX, double oneStepDistanceY)
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
        // if (distanceX < targetDistanceX)
        // {
        //     digitalWrite(STEP_PIN_X, HIGH);
        //     delayMicroseconds(DELAY);
        //     digitalWrite(STEP_PIN_X, LOW);
        //     delayMicroseconds(DELAY);
        //     distanceX += oneStepDistance;
        // }

         
        if (targetDistanceX - distanceX <= oneStepDistanceX)
        {
            oneStepDistanceX = getOneStepDistance(SIXTEENTH_STEP); // 0.01mm
            std::cout << "X MODE CHANGED TO [SIXTEENTH_STEP]\t" << "REMAINING DISTANCE X:" << targetDistanceX-distanceX <<std::endl;
        }else {
            oneStepDistanceX = getOneStepDistance(QUARTER_STEP); // 0.04mm
        }

        if (targetDistanceY - distanceY <= oneStepDistanceY)
        { 
            oneStepDistanceY = getOneStepDistance(SIXTEENTH_STEP); // 0.01mm
            std::cout << "Y MODE CHANGED TO [SIXTEENTH_STEP]\t" << "REMAINING DISTANCE Y:" << targetDistanceY-distanceY <<std::endl;
        }else {
            oneStepDistanceY = getOneStepDistance(QUARTER_STEP); // 0.04mm
        }

        if (distanceX < targetDistanceX){
            digitalWrite(STEP_PIN_X, HIGH);

        }

        if (distanceY < targetDistanceY){
            digitalWrite(STEP_PIN_Y, HIGH);
        }

        delayMicroseconds(DELAY);

        if (distanceX < targetDistanceX){
            digitalWrite(STEP_PIN_X, LOW);
            distanceX += oneStepDistanceX;
        }

        if (distanceY < targetDistanceY){
            digitalWrite(STEP_PIN_Y, LOW);
            distanceY += oneStepDistanceY;
        }

        delayMicroseconds(DELAY);
        
        // if (distanceY < targetDistanceY)
        // {
        //     delayMicroseconds(DELAY);
        //     digitalWrite(STEP_PIN_Y, LOW);
        //     delayMicroseconds(DELAY);
        //     distanceY += oneStepDistance;
        // }

        if (distanceX >= targetDistanceX && distanceY >= targetDistanceY)
        {
            std::cout <<"targetDistanceX: " << targetDistanceX << "distanceX" << distanceX << std::endl;
            std::cout <<"targetDistanceY: " << targetDistanceY << "distanceY" << distanceY << std::endl;
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