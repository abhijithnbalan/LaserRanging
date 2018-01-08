#ifndef _algorithm_h
#define _algorithm_h

#include "capture_frame.h"


class Algorithm
{
  public:
    CaptureFrame CLAHE_dehaze(CaptureFrame ); //CLAHE based basic dehazing algorithm
    cv::Mat CLAHE_dehaze(cv::Mat); //CLAHE based basic dehazing algorithm

};

#endif