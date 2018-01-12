#ifndef _laser_ranging_h
#define _laser_ranging_h
//including the necessary header files
#include "timer.h"
#include "capture_frame.h"
#include "image_processing.h"

#include <opencv2/opencv.hpp> 

//The LaserRanging class is inherited from Image processing class
class LaserRanging : public ImageProcessing
{
  private:
    float laser_distance;int centerx[2],centery[2];
    
    
    Timer timer;//For time interval and FPS calculation
    //Special for single laser 
    float left_laser_distance, right_laser_distance;
    int laser_center_x,laser_center_y;
    CaptureFrame contour_overlay_right,line_overlay_left,contour_overlay_left,line_overlay_right;

  public:
    float range,left_range,right_range;
    CaptureFrame hsv_segment,ROI,dehaze,contour_overlay,line_overlay,original;
    
    CaptureFrame contour_distance(CaptureFrame); //Contour identification and pixel distance calculation.

    CaptureFrame contour_distance_single_laser(CaptureFrame); //Contour identification and pixel distance calculation.
   
    float get_laser_distance(); //Funtion to retrieve the distance data.
    float get_left_laser_distance(); //Funtion to retrieve the distance data.
    float get_right_laser_distance(); //Funtion to retrieve the distance data.

    CaptureFrame laser_ranging(CaptureFrame); //Calling every functions for laser ranging and show it on the input image.

    CaptureFrame laser_ranging_single_laser(CaptureFrame); //Calling every functions for laser ranging and show it on the input image.

    void live_laser_ranging(CaptureFrame vid);//running laser ranging on a video
    void live_laser_ranging_single_laser(CaptureFrame vid);//running laser ranging on a video
    
    CaptureFrame show_overlay(CaptureFrame);//Overlay different display data direclt onto the image
    CaptureFrame show_overlay_single_laser(CaptureFrame);//Overlay different display data direclt onto the image
    void pixel_distance_to_distance();
    
    LaserRanging();//Constructor
};

#endif
