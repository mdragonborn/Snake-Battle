// Test environment
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include "timers.h"
//\cond
#ifdef _MSC_VER
//\endcond
#include <windows.h>

/*!
\file timers.c
\brief Fajl sa funkcijama koje mere vreme.
Credit: Marko Misic
*/

void start_timer(Timer* timer) {
    QueryPerformanceCounter(&timer->start);
}

double stop_timer(Timer* timer) {
    double elapsed;

    QueryPerformanceCounter(&timer->end);
    QueryPerformanceFrequency(&timer->frequency);

    elapsed = ((double)(timer->end.QuadPart - timer->start.QuadPart)) / ((double)(timer->frequency.QuadPart/1000.0));

	return elapsed;
}

//\cond
#else
#include <sys/time.h>

void start_timer(Timer* timer) {
    gettimeofday(&timer->start,0);
}

double stop_timer(Timer* timer) {
    double elapsed;

    gettimeofday(&timer->end,0);

    elapsed = (timer->end.tv_sec - timer->start.tv_sec)*1000.0 + (timer->end.tv_usec - timer->start.tv_usec)/1000.0;

    return elapsed;
}

#endif
//\endcond