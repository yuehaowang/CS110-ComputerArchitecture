#include "utils.h"
#include <stdio.h>
#include <time.h>

static double timer_sec;

void timer_start()
{
    timer_sec = clock();
}

void timer_end()
{
    double dt = (double)(clock() - timer_sec);
    timer_sec = 0.0;

    printf("Used Time: %f\n", dt / CLOCKS_PER_SEC);
}