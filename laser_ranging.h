#ifndef _laser_ranging_h
#define _laser_ranging_h

#include "timer.h"
#include "capture_frame.h"
#include "image_processing.h"

#include <opencv2/opencv.hpp> 

class LaserRanging : public ImageProcessing
{
  private:
    float distance;int centerx[2],centery[2];
    CaptureFrame hsv_segment,ROI,dehaze,contour_overlay,line_overlay,original;
    Timer timer;

  public:
    CaptureFrame contour_distance(CaptureFrame); //Contour identification and pixel distance calculation.
   
    float get_distance(); //Funtion to retrieve the distance data.

    CaptureFrame laser_ranging(CaptureFrame); //Calling every functions for laser ranging and show it on the input image.

    void video_processing(CaptureFrame vid);
    CaptureFrame show_overlay(CaptureFrame);
    LaserRanging();
};

#endif
