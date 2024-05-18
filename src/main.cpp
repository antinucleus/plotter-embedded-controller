#include <stdio.h>
#include "../include/core.h"
#include "../include/step.h"
#include <wiringPi.h>

int main()
{
    wiringPiSetup();
    initPins();
    disableOutput();
    setDriveMode(SIXTEENTH_STEP);
    setDriverStatus('x', DRIVER_ENABLE);
    setDriverStatus('y', DRIVER_ENABLE);

    double oneStepDistance = getOneStepDistance(SIXTEENTH_STEP);
    printf("oneStepDistance:: %f\n", oneStepDistance);

    for (;;)
    {
        printf("X axis will be move 1 cm to the right after 2 secs\n");
        delay(2000);
        moveAxis('x', 'r', oneStepDistance, 10);
        printf("X axis will be move 1 cm to the left after 2 secs\n");
        delay(2000);
        moveAxis('x', 'l', oneStepDistance, 10);
        setDriverStatus('x', DRIVER_DISABLE);

        printf("Waiting for other axis...\n");
        delay(2000);

        printf("Y axis will be move 1 cm to the forward after 2 secs\n");
        delay(2000);
        moveAxis('y', 'r', oneStepDistance, 10);
        printf("Y axis will be move 1 cm to the backward after 2 secs\n");
        delay(2000);
        moveAxis('y', 'l', oneStepDistance, 10);
        setDriverStatus('y', DRIVER_DISABLE);
        delay(2000);

        printf("End of program :)\n");

        break;
    }

    printf("Finished\n");

    return 0;
}