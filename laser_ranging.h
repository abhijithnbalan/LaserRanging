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
    int centerx[2],centery[2];
    
    Timer timer;//For time interval and FPS calculation
    //Special for single laser 
    CaptureFrame contour_overlay_right,line_overlay_left,contour_overlay_left,line_overlay_right;

  public:
    float distance_between_laser;
    float distance_ll_px, distance_rl_px, distance_px;
    int laser_center_x,laser_center_y;
    float range_mm,range_ll_mm,range_rl_mm;
    bool use_dehaze , use_dynamic_control, laser_range_status;
    CaptureFrame segmented_frame,ROI_frame,dehazed_frame,contour_overlay_frame,line_overlay_frame,original_frame;
    
    CaptureFrame contour_distance(CaptureFrame input_image); //Contour identification and pixel distance calculation.

    CaptureFrame contour_distance_single_laser(CaptureFrame input_image); //Contour identification and pixel distance calculation.
   
    float get_laser_distance_px(); //Funtion to retrieve the distance data.
    float get_left_laser_distance_px(); //Funtion to retrieve the distance data.
    float get_right_laser_distance_px(); //Funtion to retrieve the distance data.

    CaptureFrame laser_ranging(CaptureFrame input_image); //Calling every functions for laser ranging and show it on the input image.

    CaptureFrame laser_ranging_single_laser(CaptureFrame input_image); //Calling every functions for laser ranging and show it on the input image.

    void live_laser_ranging(CaptureFrame input_video);//running laser ranging on a video
    void live_laser_ranging_single_laser(CaptureFrame input_video);//running laser ranging on a video

    CaptureFrame LR_data_overlay(CaptureFrame original_image);//Overlay different display data direclt onto the image
    CaptureFrame LR_data_overlay_single_laser(CaptureFrame original_image);//Overlay different display data direclt onto the image
    void pixel_distance_to_distance();

    void image_laser_ranging(CaptureFrame input_image);
    void image_laser_ranging_single_laser(CaptureFrame input_image);

    static void laser_ranging_button(int state , void *button_user_pointer);
    void laser_ranging_handler(int state);
    float scale_L_by_px();
    float angle_of_tilt();
    void laser_ranging_calibraton(CaptureFrame);
    
    LaserRanging();//Constructor
};

#endif
