
#ifndef _view_frame_h
#define _view_frame_h
//Including necessary header files
#include "capture_frame.h"
#include <opencv2/opencv.hpp> 
#include <stdio.h>
#include <string.h>

/*
    ViewFrame class is used to show the output i.e. either image or video.
    Multiple modes of viewing is provided.
*/
class ViewFrame
{  
    private://These variables will only be used internally.
        cv::Mat image1,image2,image3,image4;

    public:
        //Single Input
        void single_view_interrupted(CaptureFrame);//Shows a single output and wait for userkey to continue
    
        void single_view_interrupted(CaptureFrame,int);//Overloaded function which also resizes the output
  
        void single_view_uninterrupted(CaptureFrame);//Shows output and continues. Used inside loops.

        void single_view_uninterrupted(CaptureFrame ,int );//Overloaded function which also resizes the output

        //Multiple outputs
        void multiple_view_interrupted(CaptureFrame , CaptureFrame );//Shows two images side by side and waits for user input.
        void multiple_view_interrupted( CaptureFrame ,CaptureFrame ,int );//Overloaded function which also resizes the output
        void multiple_view_interrupted(CaptureFrame ,CaptureFrame ,CaptureFrame );//Overloaded function to shows three images 
        void multiple_view_interrupted( CaptureFrame ,CaptureFrame,CaptureFrame,int);//Overloaded function which also resizes the output
        void multiple_view_interrupted(CaptureFrame ,CaptureFrame ,CaptureFrame ,CaptureFrame );//Overloaded function to show four outputs.

        void multiple_view_interrupted( CaptureFrame ,CaptureFrame ,CaptureFrame ,CaptureFrame ,int );//Overloaded function which also resizes the output

        //The following 3 functions are the overloaded functions of the above modified for usage in loops.
        
        void multiple_view_uninterrupted(CaptureFrame ,CaptureFrame );//2 Images

        void multiple_view_uninterrupted( CaptureFrame ,CaptureFrame ,int );//Overloaded function which also resizes the output

        void multiple_view_uninterrupted(CaptureFrame ,CaptureFrame ,CaptureFrame );//3 Images

        void multiple_view_uninterrupted( CaptureFrame ,CaptureFrame ,CaptureFrame ,int );//Overloaded function which also resizes the output

        void multiple_view_uninterrupted(CaptureFrame ,CaptureFrame ,CaptureFrame ,CaptureFrame );//4 Images

        void multiple_view_uninterrupted( CaptureFrame ,CaptureFrame ,CaptureFrame ,CaptureFrame ,int );//Overloaded function which also resizes the output
        CaptureFrame add_overlay_percent(CaptureFrame, int, int, int);
        CaptureFrame add_overlay(CaptureFrame, int, int, int);
        CaptureFrame add_overlay(CaptureFrame, int, int, std::string);
        CaptureFrame add_overlay(CaptureFrame, cv::Rect, cv::Mat);
};

#endif