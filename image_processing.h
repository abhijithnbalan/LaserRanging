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
        int x,y,width,height;
        //The following varibles(preset by Constructor) can me modified.
        
    protected://these variables will be shared in inheritance
        cv::Rect roi; bool flag;
        int hue_upper,hue_lower,saturation_upper,value_lower,lightness_upper;
        

    public:
        bool use_white;Algorithm algo;
        cv::Scalar thresh_high_0,thresh_high_180,thresh_low_0,thresh_low_180,thresh_white;
        int roi_percentage;
        cv::Mat element;

        CaptureFrame roi_selection(CaptureFrame input_image);//Selecting the Region of interst.

        cv::Mat roi_selection(cv::Mat input_image);//Selecting the Region of interst.

        CaptureFrame image_segmentation(CaptureFrame input_image);//Color segmentation. according to threshold set.

        cv::Mat image_segmentation(cv::Mat input_image);//Color segmentation. according to threshold set.

        void set_threshold(CaptureFrame current_scene_image);//Function to set the threshold value according to water type.

        void set_roi(int x_percent, int y_percent, int width, int height);// Function to set the Region of Interest 


        static void on_trackbar(int trackbar_value ,void* trackbar_user_pointer);// Function for trackbar
        static void on_trackbar_single(int trackbar_value ,void* trackbar_user_pointer);// Function for trackbar
        void myhandler(int trackbar_value);
        void myhandler_single(int trackbar_value);
        static void on_button(int trackbar_value,void* trackbar_user_pointer);// Function for button
        void myhandlerbutton(int trackbar_value);

        ImageProcessing();//Constructor definition The values are preset in this constructor.

};

#endif