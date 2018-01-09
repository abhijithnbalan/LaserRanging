#ifndef _capture_frame_h
#define _capture_frame_h

#include <opencv2/opencv.hpp> 
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <string.h>
/*  
    CaptureFrame class will be used as an instace of Image.
*/
class CaptureFrame
{
    protected://Acessing these variables can only be through member functions
        cv::Mat image;//Stores Image file
        cv::VideoCapture cap;//Stores VideoCapture file
        
    public:
        std::string window_name;// Window name of stored image 

        //load_image is used to load data to variable image
        void capture_image(std::string ,std::string);

        //load_video is used to load VideoCapture data to variable cap
        void capture_video(std::string ,std::string );

        //load a image file into an existing CaptureFrame object
        void reload_image(cv::Mat,std::string);

        //get_image and get_video are used to retrieve data.
        cv::Mat retrieve_image();
        cv::VideoCapture retrieve_video();

        void frame_extraction();//Extract frames from video

        void clear();//Clears and releases the memory allocated for variables

        // Constrructors that will take image file and window name
        CaptureFrame(cv::Mat,std::string);
        //Constructor that will initialise every varibles to be empty.
        CaptureFrame();
};

#endif
