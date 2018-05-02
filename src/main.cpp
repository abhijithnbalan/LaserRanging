

//Including the codes for functions
#include "capture_frame.h"
#include "view_frame.h"
#include "timer.h"
#include "image_processing.h"
#include "algorithm.h"
#include "laser_ranging.h"
#include "logger.h"
//standard libraries
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cstdlib> //for integer checking support
#include <unistd.h>
int main(int argc, char **argv) //The main Function
{
    int success = chdir("..");
    Logger logger;
    logger.log_info("Laser Ranging starts");
    std::cout<<"1\n";
    logger.log_debug("Working directory switched to one directory back");
   
    //Changing directory for accessing files. another workaround is giving full path for each files.
    
    if(success != 0)
    {
        logger.log_error("Unable to change working directory");
        std::cout<<"couldn't change the directory/\n";
        return -1;    
    }
  
    LaserRanging Ranger;
    cv::Mat image_stream;

    int x, y, wt, ht, hue_high, hue_low, sat_high, sat_low, val_high, val_low;//for changing roi in image_stream
    bool dev_mode = false;//developer mode control
    bool exe_mode = false;//execution mode control

    if(argc > 1)//checking for exe
    {
        if (!strcmp(argv[1], "Exe") || !strcmp(argv[1], "exe"))
        {
            exe_mode = true;
        }
    }
    
    if (!exe_mode)
    {
        logger.log_debug("Developer mode selected");
        std::string filename; // checking the extension if argument passed is string
        bool camera = false;  //for camera the argument passed will be integer

        if (argc > 1) //some argument is passed in dev mode
        {
            if (!strcmp(argv[1], "Dev") || !strcmp(argv[1], "dev"))
            {

                dev_mode = true;
                Ranger.dev_mode = true;
                if(argc>2)
                {
                    char *end;
                std::istringstream in(argv[argc - 1]);
                int test;
                if (in >> test && in.eof())
                    camera = true;
                else
                    filename = argv[argc - 1]; 
                }
                else
                {
                    filename = "samples/rendered.mp4";
                    std::cout << "User input not detected. Default path " << filename << " is used.\n\n";
                }
            }
            //checking if the argument is integer or string. in case of integer, camera will be initiated.
            else
            {
                char *end;
                std::istringstream in(argv[argc - 1]);
                int test;
                if (in >> test && in.eof())
                    camera = true;
                else
                    filename = argv[argc - 1]; //argument is not integer so filename is recorded as string.
            }
        }

        else //no arguments passed. using default path
        {
            filename = "samples/rendered.mp4";
            std::cout << "User input not detected. Default path " << filename << " is used.\n\n";
        }

        std::string extension = filename.substr(filename.find_last_of(".") + 1); //extracting extension from filename.
        if (extension == "mp4" || extension == "avi" || extension == "flv" ||    //checking extension to video and also camera
            extension == "mpg" || extension == "mkv" || camera)                  //check mkv
        {
            //----------VIDEO------------//
            CaptureFrame vid;
            if (camera)
            {
                logger.log_info("Initiated Camera");
                vid.capture_video(atoi(argv[argc - 1]), "Input"); //In case of camera
            }
            else
            {
                logger.log_info("Initiated with Video input");
                vid.capture_video(filename, "Input"); //In case of video filename
            }

            // Ranger.use_dehaze = true;
            // Ranger.algo.set_CLAHE_clip_limit(2);
            // Ranger.use_white = true;
            // Ranger.use_dynamic_control = false;
            // Ranger.laser_range_status = false;
            logger.log_info("Initiated Laser Ranging");
            Ranger.live_laser_ranging_single_laser(vid);

            //---------------------------//
        }
        else if (extension == "png" || extension == "jpg" || extension == "jpeg") //checing with image extension
        {
            // ---------IMAGE------------//
            CaptureFrame img;
            img.capture_image(filename, "image");
            // Ranger.use_dehaze = true;
            // Ranger.algo.set_CLAHE_clip_limit(2);
            // Ranger.use_white = true;
            Ranger.use_dynamic_control = false;
            // Ranger.laser_range_status = false;
            logger.log_info("Initiated Image mode");
            Ranger.image_laser_ranging_single_laser(img);

            //----------------------------//
        }
        else
        {
            logger.log_error("Filetype entered is unknown");
            std::cout << "\n>>The file type is unknown. Please use the standard types\n {mp4,avi,mpeg,flv,jpg,jpeg,png or camera port}<<\n\n";
        }
    }
    else
    {
        image_stream = cv::imread("samples/test1.png", 1);
        // Ranger.set_roi(x,y,wt,ht);
        // Ranger.set_threshold(hue_low,hue_high,sat_low,sat_high,val_low,val_high);
        logger.log_info("Initiated Image stream mode");
        Ranger.image_stream_laser_ranging_single_laser(image_stream, 0);
        cv::waitKey(3);
    }
    usleep(1000000);
    logger.log_info("Program ended");
    
    return 1;

}