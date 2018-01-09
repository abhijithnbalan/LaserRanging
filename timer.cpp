#include "timer.h"
#include "capture_frame.h"
#include "view_frame.h"
#include <time.h>

        void Timer::timer_init()//Function which starts the timer
        {
            time_start = clock();
            return;
        }
        void Timer::timer_end()//Function to end the timer and calculate the interval
        {
            time_end = clock();
            fps =CLOCKS_PER_SEC/((float)time_end - (float)time_start);
            execution_time =((float)time_end - (float)time_start)/CLOCKS_PER_SEC;
            return;
        }

        CaptureFrame Timer::add_time(CaptureFrame object)//Function to print the execution time on image
        {
            cv::Mat image = object.retrieve_image();
            std::ostringstream sst;
            sst << execution_time;
            std::string s(sst.str());
            putText(image,s, cvPoint(30,30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);//Adding time into frame
            CaptureFrame output(image,"output with time data");
            return output;
        }
        CaptureFrame Timer::add_fps(CaptureFrame object)//Function to print the execution time on image
        {
            ViewFrame viewer;
            CaptureFrame output = viewer.add_overlay_percent(object,10,10,fps);
            return output;
        }