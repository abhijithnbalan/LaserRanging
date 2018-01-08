#include <opencv2/opencv.hpp> 
#include <stdio.h>
#include <string.h>
#include "capture_frame.h"


void CaptureFrame::capture_image(std::string filename,std::string image_window_name)
    {
        image = cv::imread(filename,1);
        if ( !image.data )
        {
            std::cout<<"No image data found for "<<filename<<std::endl;// no input image found
            exit(0);
        }
        window_name = image_window_name;
        return;
    }
void CaptureFrame::capture_video(std::string filename,std::string video_window_name)
    {
        cap.open(filename);
        if(!cap.isOpened())  // check if we succeeded
        {
            printf("Video is not opened..:(");
            exit(0);
        }
        window_name = video_window_name;
    }
void CaptureFrame::reload_image(cv::Mat image_input,std::string str)
    {
        if ( !image_input.data )
        {
            std::cout<<"No image data found for loading for "<<str<<std::endl;// no input image found
            exit(0);
        }
        image = image_input;
        window_name = str;
    }
cv::Mat CaptureFrame::retrieve_image()
    {
        return image;
    }
cv::VideoCapture CaptureFrame::retrieve_video()
    {
        return cap;
    }
void CaptureFrame::frame_extraction()//Extract frames from video
    {
        cap>>image;
        if ( !image.data )
        {
            std::cout<<"No image data found for "<<window_name<<std::endl;// no input image found
            exit(0);
        }//After this function call the current frame is saved in the image file of the same object.
        return;
    }
CaptureFrame::CaptureFrame(cv::Mat input,std::string window)
    {
        image = input;
        window_name = window;
    }
CaptureFrame::CaptureFrame()
{}