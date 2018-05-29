
#include "laser_calibration.h"
#include "capture_frame.h"
#include "view_frame.h"
#include <stdio.h>
#include <unistd.h> //For Directory changing.
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> //opencv support
#include "logger.h"
//for json support -- rapidjason is used
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include <fstream>

int main(int argc, char **argv)
{
    bool debug_mode, single_laser_status;
    std::string running_mode, execution_mode, default_input;
    int roi_x, roi_y, roi_height, roi_width;
    int hue_high, hue_low, sat_high, sat_low, val_high, val_low; //for changing roi in image_stream
    bool dev_mode = false;                                       //developer mode control
    bool exe_mode = false;                                       //execution mode control
    bool use_white, use_dehaze, dynamic_control;
    int calibration_distance;

    //Changing directory for accessing files. another workaround is giving full path for each files.
    int success = chdir("..");
    Logger logger;
    // logger.log_info("Calibration starts");

    if (success != 0)
    {
        logger.log_error("Unable to change the working directory");
        std::cout << "couldn't change the directory/\n";
        return -1;
    }
    logger.log_debug("Working directory changed to one directory back");

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

        calibration_distance = configuration_file["Laser Calibration Settings"]["CalibrationDistance"].GetInt();
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
           "The Default input file is %s\n"
           "Laser Ranging Settings\n"
           "Single Laser status is %d\n"
           "White use status is %d\n"
           "Dehaze use status is %d\n"
           "Dynamic Control status is %d\n"
           "Hue high limit is %d and low limit is %d\n"
           "Saturarion high limit is %d and low limit is %d\n"
           "Value high limit is %d and low limit is %d\n\n",
           running_mode.c_str(), execution_mode.c_str(), debug_mode, roi_x, roi_y, roi_width, roi_height, default_input.c_str(), single_laser_status, use_white, use_dehaze, dynamic_control, hue_high, hue_low, sat_high, sat_low, val_high, val_low);

    LaserCalibration calibrate;
    cv::Mat image_stream;
    calibrate.debug_mode = debug_mode;
    calibrate.use_dehaze = use_dehaze;
    calibrate.use_white = use_white;
    calibrate.use_dynamic_control = dynamic_control;
    calibrate.set_roi(roi_x, roi_y, roi_width, roi_height);
    calibrate.set_threshold(hue_low, hue_high, sat_low, sat_high, val_low, val_high);

    logger.debug_mode = debug_mode;
    if (running_mode == "commandline")
    {
        if (execution_mode == "DEV")
        {
            dev_mode = true;
        }
        else if (execution_mode == "EXE")
        {
            exe_mode = true;
        }
        else
            logger.log_warn("Undefined execution mode");

        if (!exe_mode)
        {
            logger.log_info("DEV mode enabled");

            std::string filename; // checking the extension if argument passed is string
            bool camera = false;  //for camera the argument passed will be integer

            if (dev_mode)
            {
                calibrate.dev_mode = true;

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
                    logger.log_info("Camera initiated");
                    vid.capture_video(atoi(argv[argc - 1]), "Input"); //In case of camera
                }
                else
                {
                    logger.log_info("Initiating with video");
                    vid.capture_video(filename, "Input"); //In case of video filename
                }

                calibrate.calibration_distance = calibration_distance;
                logger.log_info("Entering to Laser Ranging");
                calibrate.laser_ranging_calibration(vid);

                //---------------------------//
            }
            else if (extension == "png" || extension == "jpg" || extension == "jpeg") //checing with image extension
            {
                // ---------IMAGE------------//
                CaptureFrame img;
                img.capture_image(filename, "image");

                logger.log_info("Entering to image stream laser Ranging");
                calibrate.image_stream_laser_ranging_calibration(img);

                //----------------------------//
            }
            else
            {
                logger.log_error("Unknown file type entered");
                std::cout << "\n>>The file type is unknown. Please use the standard types\n {mp4,avi,mpeg,flv,jpg,jpeg,png or camera port}<<\n\n";
            }
        }
        else
        {
            image_stream = cv::imread("samples/test1.png", 1);
            calibrate.image_stream_laser_ranging_calibration(image_stream, 0);
            cv::waitKey(3);
        }
    }
    usleep(1000);
    return 1;
}