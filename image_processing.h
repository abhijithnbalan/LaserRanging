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
        cv::Mat element,inter;
        cv::Mat image_hsv,image_hsv_threshold,image_hsv_threshold_low,image_hsv_threshold_high;
        //The following varibles(preset by Constructor) can me modified.
        cv::Scalar thresh_high_0,thresh_high_180,thresh_low_0,thresh_low_180;
    protected://these variables will be shared in inheritance
        int roi_percentage;cv::Rect roi; Algorithm algo;

    public:
        CaptureFrame roi_selection(CaptureFrame);//Selecting the Region of interst. full width is taken.

        CaptureFrame hsv_segmentation(CaptureFrame );//Color segmentation. according to threshold set.

        cv::Mat roi_selection(cv::Mat);//Selecting the Region of interst. full width is taken.

        cv::Mat hsv_segmentation(cv::Mat );//Color segmentation. according to threshold set.

        void set_threshold(CaptureFrame );//Function to set the threshold value according to water type.

        void set_roi(int );// Function to set the Region of Interest 

        ImageProcessing();//Constructor definition The values are preset in this constructor.

};

#endif