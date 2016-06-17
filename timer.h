#ifndef _timers_h_
#define _timers_h_
//\cond
#ifdef _MSC_VER
//\endcond
#include <windows.h>

/*!
\file timers.h
\brief Header za funkcije koje mere vreme.
Credit: Marko Misic
*/

/*!
\typedef struct timer Timer
\brief Struktura koja u sebi sadrzi vreme

*/
typedef
/*!
\struct Timer
\brief Struktura koja u sebi sadrzi vreme

*/
struct Timer{
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    LARGE_INTEGER frequency;
} Timer;

void start_timer(Timer* timer);
double stop_timer(Timer* timer);

//\cond
#else
#include <sys/time.h>

typedef struct {
    struct timeval start;
    struct timeval end;
} Timer;

void start_timer(Timer* timer);
double stop_timer(Timer* timer);

#endif

#endif
//\endcond