#include "laser_calibration.h"
#include "laser_ranging.h"
//for json support -- rapidjason is used
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include <fstream>

void LaserCalibration::laser_ranging_calibration(CaptureFrame vid)
{
    logger.log_info("Calibration initiated");
    //opening the calibration json file and reading the existing values
    read_from_json("laser_calibration_values.json","all");
    std::cout << "The current values : x = " << laser_center_x
              << "  y = " << laser_center_y << "\n"<<" calibration_distance : "<<calibration_distance<<"\n";

    ViewFrame viewer;
    std::cout << "Calibration initiated\n";
    std::cout << "Press c to record laser center values  Press ESC to exit\n";
    logger.log_debug("Entering into calibration loop");
    CaptureFrame outframe,final_output;
    cv::Mat temporary;
    for (;;)
    {
        vid.frame_extraction();
        // temporary.release();
        outframe.clear();
        outframe = roi_selection(vid);
        outframe = image_segmentation(outframe);
        outframe = contour_distance(outframe);
        // outframe.reload_image(temporary,"laser raning");
        final_output = calibration_overlay(vid);
        if(dev_mode)viewer.multiple_view_uninterrupted(final_output,outframe);
        char c = (char)cv::waitKey(20);//delay is longer for the ease of capturing.
        if (c == 99 || calib_trigger)//Recording user key press 'c' or the trigger becomes true
        {
            if ((centerx[1] + centery[1]) == 0 || (centerx[0] + centery[0]) == 0)
            {   
                //Two laser dots not found so using the existng values in json file.
                std::cout << "Could not find two laser points successfully\nCalibration failed\nExiting";
            }
            else
            {
                //two laser dots are identified
                laser_center_x = (centerx[0] + centerx[1]) / 2;
                laser_center_y = (centery[0] + centery[1]) / 2;
                parallax_constant = calibration_distance*(distance_rl_px + distance_ll_px);
                std::cout << "Capturing laser center values as  x = " << laser_center_x
                          << " and  y = " << laser_center_y <<" for distance "<<calibration_distance<< "\n";
                //now writing the new calibraiton values to json file
                write_to_json("laser_calibration_values.json","all");

            }
            calib_trigger = false;
            if(dev_mode)cv::destroyWindow(outframe.window_name );
            break;
        }
        else if (c == 27 || calib_cancel)//userkey ESC is pressed
        {
            //skipping calibration
            if(dev_mode)cv::destroyWindow(outframe.window_name );
            calib_cancel = false;
            break;
        }
    }
    logger.log_debug("Exited from calibration loop");
    return;
}

void LaserCalibration::image_stream_laser_ranging_calibration(CaptureFrame vid)
{
    
    //opening the calibration json file and reading the existing values
    read_from_json("laser_calibration_values.json","all");
    ViewFrame viewer;
    std::cout << "\rPress c to record laser center values  Press ESC to exit\n";
    CaptureFrame outframe;
    // cv::Mat temporary;
        outframe = laser_ranging_single_laser(vid);//laser ranging with single laser
        // outframe.reload_image(temporary,"laser raning");
        //laser ranging with single laser
        if(dev_mode)viewer.single_view_uninterrupted(outframe,50);
        char c = (char)cv::waitKey(100);//delay is longer for the ease of capturing.
        if (c == 99 || calib_trigger)//Recording user key press 'c' or the trigger becomes true
        {
            logger.log_debug("Calibration triggered");
            if ((centerx[1] + centery[1]) == 0 || (centerx[0] + centery[0]) == 0)
            {   
                logger.log_info("Calibration completed unsuccessfully");
                //Two laser dots not found so using the existng values in json file.
                std::cout << "Could not find two laser points successfully\nCalibration failed\nExiting";
            }
            else
            {
                logger.log_info("Calibration completed successfully.");
                //two laser dots are identified
                laser_center_x = (centerx[0] + centerx[1]) / 2;
                laser_center_y = (centery[0] + centery[1]) / 2;
                std::cout << "Capturing laser center values as  x = " << laser_center_x
                          << " and  y = " << laser_center_y << "\n";
                //now writing the new calibraiton values to json file
                write_to_json("laser_calibration_values.json","all");
            }
            calib_trigger = false;
            if(dev_mode)cv::destroyWindow("Resized Image");
        }
        else if (c == 27 || calib_cancel)//userkey ESC is pressed
        {
            //skipping calibration
            logger.log_info("Skipping calibration");
            if(dev_mode)cv::destroyWindow("Resized Image");
            calib_cancel = false;
        
        }
    
    return;
}

void LaserCalibration::image_stream_laser_ranging_calibration(CaptureFrame vid, int mode)
{
    logger.log_info("Minimal calibration for image stream initiated");
    //opening the calibration json file and reading the existing values
    read_from_json("laser_calibration_values.json","all");
    ViewFrame viewer;
    std::cout << "Calibration initiated\n";
    std::cout << "Press c to record laser center values  Press ESC to exit\n";
    // cv::Mat temporary;
    CaptureFrame outframe;
        outframe = laser_ranging_single_laser(vid);//laser ranging with single laser
        // outframe.reload_image(temporary,"laser raning");
        //laser ranging with single laser
        char c = (char)cv::waitKey(100);//delay is longer for the ease of capturing.
        if (c == 99 || calib_trigger)//Recording user key press 'c' or the trigger becomes true
        {
            logger.log_debug("dCalibration triggered");
            if ((centerx[1] + centery[1]) == 0 || (centerx[0] + centery[0]) == 0)
            {   
                logger.log_info("Calibration completed unsuccessfully");
                //Two laser dots not found so using the existng values in json file.
                std::cout << "Could not find two laser points successfully\nCalibration failed\nExiting";
            }
            else
            {
                logger.log_info("Calibration completed successfully");
                //two laser dots are identified
                laser_center_x = (centerx[0] + centerx[1]) / 2;
                laser_center_y = (centery[0] + centery[1]) / 2;
                std::cout << "Capturing laser center values as  x = " << laser_center_x
                          << " and  y = " << laser_center_y << "\n";
                //now writing the new calibraiton values to json file
                write_to_json("laser_calibration_values.json","all");
            }
            calib_trigger = false;
        }
        else if (c == 27 || calib_cancel)//userkey ESC is pressed
        {
            //skipping calibration
            calib_cancel = false;
        
        }
    
    return;
}

void LaserCalibration::image_stream_laser_ranging_calibration(cv::Mat image_stream, int mode)
{
    logger.log_info("Minimal calibration for image stream initiated");
    CaptureFrame input(image_stream,"image stream input");
    image_stream_laser_ranging_calibration(input,mode);
    return;
}

void LaserCalibration::write_to_json(std::string filename, std::string all)
{
    if(all != "all")
    {
        logger.log_error("Unknown Writing mode for json");
        std::cout<<"unknown writing mode\n";
        std::cout<<"Error occured in writing to json file\n";
        return;
    }
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    std::cout<<extension<<"  "<<filename<<"\n";
    if(extension !="json")
    {
        logger.log_error("Incorrect file type for json file");
        std::cout<<"Given file is not a json file. Please provide a json file\n";
        exit(0);
    }
    try{
    std::ifstream ifs("laser_calibration_values.json");
    if(ifs.fail())throw(20);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document calibration_file;
    calibration_file.ParseStream(isw);
    rapidjson::Value &document_x_value = calibration_file["laser_center_x"];
    rapidjson::Value &document_y_value = calibration_file["laser_center_y"];
    document_x_value.SetInt(laser_center_x);
    document_y_value.SetInt(laser_center_y);
    std::cout << "The calibrted values : x = " << calibration_file["laser_center_x"].GetInt()
        << "  y = " << calibration_file["laser_center_y"].GetInt() << "\n\n";
    std::ofstream ofs(filename);
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    calibration_file.Accept(writer);
    }
    catch(...)
    {
        logger.log_error("Unable to write to json file");
        return;
    }
    return;
}


LaserCalibration::LaserCalibration()
{
    logger.log_debug("Laser Calibration constructor");
    centerx[0] = centerx[1] = 0; //Initialising all centers and ranges to be zeros
    centery[0] = centery[1] = 0; 

    //For initializing the laser center point, the json file is opended and the recorded data is read.
    read_from_json("laser_calibration_values.json","all");
    dev_mode = false;//developer mode is disabled unless specified otherwise.
    distance_between_laser = 100;//the distance between laser pointers
    calibration_status = true;//calibration is used by default
    calib_trigger = false;//calibration trigger set to false by default
    calib_cancel = false;//calibration cancellation variable set to false by default
}


CaptureFrame LaserCalibration::calibration_overlay(CaptureFrame input)
{
    cv::Mat img = input.retrieve_image().clone();
    if(centerx[0] + centerx[1] + centery[0] + centery[1] != 0)
    {
        // logger.log_warn("line done");
        cv::line(img,cv::Point2d(0,roi.y + (centery[0] + centery[1])/2),cv::Point2d(img.cols - 1,roi.y + (centery[0] + centery[1])/2),cv::Scalar(0,255,0));
        cv::line(img,cv::Point((centerx[0] + centerx[1])/2,0),cv::Point((centerx[0] + centerx[1])/2,img.rows - 1),cv::Scalar(0,255,0));
        
        input.clear();
        input.reload_image(img.clone(),"Calibration Overlay");
        img.release();
        cv::waitKey(150);

    }
    return input;

}