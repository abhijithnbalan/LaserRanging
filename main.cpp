

//Including the codes for functions
#include "capture_frame.h"
#include "view_frame.h"
#include "timer.h"
#include "image_processing.h"
#include "algorithm.h"
#include "laser_ranging.h"
//standard libraries
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cstdlib>//for integer checking support

int main(int argc, char **argv) //The main Function
{
    std::string filename;// checking the extension if argument passed is string
    bool camera = false;//for camera the argument passed will be integer

    if(argc == 2)//some argument is passed
    {   
        //checking if the argument is integer or string. in case of integer, camera will be initiated.
        char* end;
        std::istringstream in(argv[1]);
        int test;
        if(in >> test && in.eof())camera = true;
        else filename = argv[1];//argument is not integer so filename is recorded as string.
    }
    else//no arguments passed. using default path
    {   
        filename = "rendered.mp4";
        std::cout<<"User input not detected. Default path "<<filename<<" is used.\n\n";
    }

    LaserRanging Ranger;//Laser ranging object
    
    std::string extension = filename.substr(filename.find_last_of(".") + 1);//extracting extension from filename.
    if (extension == "mp4" || extension == "avi" || extension == "flv" || //checking extension to video and also camera
        extension == "mpg" || extension == "mkv" || camera) //check mkv
    {
        //----------VIDEO------------//
        CaptureFrame vid;
        if (camera)
            vid.capture_video(atoi(argv[1]), "Input"); //In case of camera
        else
            vid.capture_video(filename, "Input"); //In case of video filename
        // Ranger.use_dehaze = true;
        // Ranger.algo.set_CLAHE_clip_limit(2);
        // Ranger.use_white = true;
        // Ranger.use_dynamic_control = false;
        // Ranger.laser_range_status = false;

        Ranger.live_laser_ranging_single_laser(vid);

        //---------------------------//
    }
    else if (extension == "png" || extension == "jpg" || extension == "jpeg")//checing with image extension
    {
        // ---------IMAGE------------//
        CaptureFrame img;
        img.capture_image(filename, "image");
        // Ranger.use_dehaze = true;
        // Ranger.algo.set_CLAHE_clip_limit(2);
        // Ranger.use_white = true;
        Ranger.use_dynamic_control = false;
        // Ranger.laser_range_status = false;
        Ranger.image_laser_ranging_single_laser(img);

        //----------------------------//
    }
    else
    {
        std::cout<<"\n>>The file type is unknown. Please use the standard types\n {mp4,avi,mpeg,flv,jpg,jpeg,png or camera port}<<\n\n";
    }
    return 1;
}