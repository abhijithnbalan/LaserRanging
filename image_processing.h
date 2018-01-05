#ifndef _image_processing_h
#define _image_processing_h

#include <opencv2/opencv.hpp> 
// #include "opencv2/imgproc/imgproc.hpp"
#include "capture_frame.h"

/*
    ImageProcessing class will be used for all processing operations on the image or image steam.
*/
class ImageProcessing //-------medianBlur or CLAHE dehazing-------//
{
    private://These variables will be used internally
        cv::Mat element;float distance;
        cv::Mat image_hsv,image_hsv_threshold,image_hsv_threshold_low,image_hsv_threshold_high,inter;
        //The following varibles(preset by Constructor) can me modified.
        cv::Scalar thresh_high_0,thresh_high_180,thresh_low_0,thresh_low_180;
        int roi_percentage;cv::Rect roi;

    public:
        CaptureFrame roi_selection(CaptureFrame);//Selecting the Region of interst. full width is taken.

        CaptureFrame hsv_segmentation(CaptureFrame );//Color segmentation. according to threshold set.

        CaptureFrame contour_distance(CaptureFrame );//Contour identification and pixel distance calculation.

        float get_distance();//Funtion to retrieve the distance data.

        CaptureFrame laser_ranging(CaptureFrame );//Calling every functions for laser ranging and show it on the input image.

        void set_threshold(CaptureFrame );//Function to set the threshold value according to water type.

        void set_roi(int );// Function to set the Region of Interest 

        CaptureFrame CLAHE_dehaze(CaptureFrame );//CLAHE based basic dehazing algorithm
 
        void video_processing(CaptureFrame );

        ImageProcessing();//Constructor definition The values are preset in this constructor.

};

#endif