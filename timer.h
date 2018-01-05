#ifndef _timer_h
#define _timer_h

#include "capture_frame.h"
#include <time.h>
/*
    Timer class is used to find the execution time of the program
*/
class Timer
{
    private:
        clock_t time_start,time_end;//Variables for time interval calculation
    public:
        float execution_time;//Output is public so that it can be accessed easily
        
        void timer_init();//Function which starts the timer

        void timer_end();//Function to end the timer and calculate the interval

        CaptureFrame add_time(CaptureFrame );//Function to print the execution time on image

};

#endif