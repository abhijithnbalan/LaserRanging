#ifndef _image_processing_h
#define _image_processing_h

//Including the essential header files
#include <opencv2/opencv.hpp> 
#include "capture_frame.h"
#include "algorithm.h"

/*
    ImageProcessing class will be used for all processing operations on the image or image steam.
*/
class ImageProcessing //-------medianBlur or CLAHE dehazing-------//
{
    private://These variables will be used internally
        cv::Mat image_hsv,inter,image_hsv_threshold,image_hsv_threshold_low,image_hsv_threshold_high,image_hsv_threshold_white;
    
        //The following varibles(preset by Constructor) can me modified.
        
    protected://these variables will be shared in inheritance
        cv::Rect roi; Algorithm algo;bool flag;
        int hue_upper,hue_lower,saturation_upper,value_lower,lightness_upper;
        

    public:
        bool white_use;
        cv::Scalar thresh_high_0,thresh_high_180,thresh_low_0,thresh_low_180,thresh_white;
        int roi_percentage;
        cv::Mat element;
        CaptureFrame roi_selection(CaptureFrame);//Selecting the Region of interst. full width is taken.

        CaptureFrame hsv_segmentation(CaptureFrame );//Color segmentation. according to threshold set.

        cv::Mat roi_selection(cv::Mat);//Selecting the Region of interst. full width is taken.

        cv::Mat hsv_segmentation(cv::Mat );//Color segmentation. according to threshold set.

        void set_threshold(CaptureFrame );//Function to set the threshold value according to water type.

        void set_roi(int );// Function to set the Region of Interest 

        static void on_trackbar(int,void*);// Function for trackbar
        static void on_trackbar_single(int,void*);// Function for trackbar
        void myhandler(int);
        void myhandler_single(int);
        static void on_button(int,void*);// Function for button
        void myhandlerbutton(int);

        ImageProcessing();//Constructor definition The values are preset in this constructor.

};

#endif