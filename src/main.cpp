

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
//for json support -- rapidjason is used
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include <fstream>

int main(int argc, char **argv) //The main Function
{
    bool debug_mode, single_laser_status,image_stream_mode;
    std::string running_mode, execution_mode, default_input;
    int roi_x, roi_y, roi_height, roi_width;
    int hue_high, hue_low, sat_high, sat_low, val_high, val_low; //for changing roi in image_stream
    bool dev_mode = false;                                       //developer mode control
    bool exe_mode = false;                                       //execution mode control
    bool use_white, use_dehaze, dynamic_control;
    int distance_between_lasers;

    int success = chdir("..");
    Logger logger;
    logger.log_info("Laser Ranging  ");
    logger.log_debug("Working directory switched to one directory back");

    //Changing directory for accessing files. another workaround is giving full path for each files.

    if (success != 0)
    {
        logger.log_error("Unable to change working directory");
        std::cout << "couldn't change the directory/\n";
        return -1;
    }

    try
    {
        std::ifstream ifs("configure.json");

        if (ifs.fail())
            throw(20);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document configuration_file;
        configuration_file.ParseStream(isw);

        debug_mode = configuration_file["Program Execution"]["DebugMode"].GetBool();
        running_mode = configuration_file["Program Execution"]["RunningMode"].GetString(); //Reading data from json
        execution_mode = configuration_file["Program Execution"]["ExeMode"].GetString();

        roi_x = configuration_file["General Settings"]["RegionOfInterest_x"].GetInt();
        roi_y = configuration_file["General Settings"]["RegionOfInterest_y"].GetInt();
        roi_width = configuration_file["General Settings"]["RegionOfInterest_width"].GetInt();
        roi_height = configuration_file["General Settings"]["RegionOfInterest_height"].GetInt();
        default_input = configuration_file["General Settings"]["DefaultInput"].GetString();

        image_stream_mode = configuration_file["Laser Ranging Settings"]["ImageStreamMode"].GetBool();
        single_laser_status = configuration_file["Laser Ranging Settings"]["SingleLaser"].GetBool();
        use_white = configuration_file["Laser Ranging Settings"]["UseWhite"].GetBool();
        use_dehaze = configuration_file["Laser Ranging Settings"]["UseDehaze"].GetBool();
        dynamic_control = configuration_file["Laser Ranging Settings"]["DynamicControl"].GetBool();
        hue_high = configuration_file["Laser Ranging Settings"]["HueHighLimit"].GetInt();
        hue_low = configuration_file["Laser Ranging Settings"]["HueLowLimit"].GetInt();
        sat_high = configuration_file["Laser Ranging Settings"]["SaturationHighLimit"].GetInt();
        sat_low = configuration_file["Laser Ranging Settings"]["SaturationLowLimit"].GetInt();
        val_high = configuration_file["Laser Ranging Settings"]["ValueHighLimit"].GetInt();
        val_low = configuration_file["Laser Ranging Settings"]["ValueLowLimit"].GetInt();
        distance_between_lasers = configuration_file["Laser Ranging Settings"]["DistanceBetweenLasers"].GetInt();

    }
    catch (...)
    {
        logger.log_error("Unable to open or read json file");
        std::cout << "Error in file opening or reading \"laser_calibration_values.json\" file\n";
        exit(0);
    }

    printf("\n\n\t\t\tConfiguration File\n\n"
           "Execution Settings\n"
           "Running the Program in %s in %s with Debug status : %d \n\n"
           "General Settings\n"
           "The Region of Interset is selected as (%d,%d,%d,%d)\n\n"
           "The Default input file is %s\n\n"
           "Laser Ranging Settings\n"
           "Image Stream Mode status is %d\n"
           "Single Laser status is %d\n"
           "White use status is %d\n"
           "Dehaze use status is %d\n"
           "Dynamic Control status is %d\n"
           "Hue high limit is %d and low limit is %d\n"
           "Saturarion high limit is %d and low limit is %d\n"
           "Value high limit is %d and low limit is %d\n"
           "The distance between laser pointers are taken as %d\n\n",
           running_mode.c_str(), execution_mode.c_str(), debug_mode, roi_x, roi_y, roi_width, roi_height, default_input.c_str(),image_stream_mode, single_laser_status, use_white, use_dehaze, dynamic_control, hue_high, hue_low, sat_high, sat_low, val_high, val_low,distance_between_lasers);

    LaserRanging Ranger;
    cv::Mat image_stream;

    Ranger.debug_mode = debug_mode;
    Ranger.use_dehaze = use_dehaze;
    Ranger.use_white = use_white;
    Ranger.use_dynamic_control = dynamic_control;
    Ranger.set_roi(roi_x, roi_y, roi_width, roi_height);
    Ranger.set_threshold(hue_low, hue_high, sat_low, sat_high, val_low, val_high);
    Ranger.distance_between_laser = distance_between_lasers;

    logger.debug_mode = debug_mode;

    if (execution_mode == "DEV")
    {
        dev_mode = true;
    }
    else if (execution_mode == "EXE")
    {
        dev_mode = false;
    }
    else
        logger.log_warn("Undefined execution mode");

    if (running_mode == "commandline")
    {

        if (!image_stream_mode)
        {
            logger.log_debug("Developer mode selected");
            std::string filename; // checking the extension if argument passed is string
            bool camera = false;  //for camera the argument passed will be integer

            if (dev_mode)
            {
                Ranger.dev_mode = true;

                if (argc > 1)
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
                    filename = default_input;
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

                if (single_laser_status)
                {
                    logger.log_info("Initiated Single Laser Mode");
                    Ranger.live_laser_ranging_single_laser(vid);
                }
                else
                    Ranger.live_laser_ranging(vid);

                //---------------------------//
            }
            else if (extension == "png" || extension == "jpg" || extension == "jpeg") //checing with image extension
            {
                // ---------IMAGE------------//
                CaptureFrame img;
                img.capture_image(filename, "image");

                logger.log_info("Initiated Image mode");

                if (single_laser_status)
                {
                    logger.log_info("Single Laser Mode");
                    Ranger.image_laser_ranging_single_laser(img);
                }
                else
                    Ranger.image_laser_ranging(img);

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

            logger.log_info("Initiated Image stream mode");
            if (single_laser_status)
            {
                logger.log_info("Single Laser Mode");
                Ranger.image_stream_laser_ranging_single_laser(image_stream, 0);
            }
            else
                Ranger.image_stream_laser_ranging(image_stream, 0);
            cv::waitKey(3);
        }
        usleep(1000000);
        logger.log_info("Program ended");
    }

    else if (running_mode == "GUI")
    {
        //Code for GUI running mode goes here
    }
    else if (running_mode == "automated")
    {
        //Code for automated running mode goes here
    }
    else
        logger.log_error("Undefined Running Mode");

    return 1;
}