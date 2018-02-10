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
    //opening the calibration json file and reading the existing values
    read_from_json("laser_calibration_values.json","all");
    std::cout << "The current values : x = " << laser_center_x
              << "  y = " << laser_center_y << "\n"<<" calibration_distance : "<<calibration_distance<<"\n";

    ViewFrame viewer;
    std::cout << "Calibration initiated\n";
    std::cout << "Press c to record laser center values  Press ESC to exit\n";
    for (;;)
    {
        vid.frame_extraction();
        CaptureFrame outframe = laser_ranging_single_laser(vid);//laser ranging with single laser
        if(dev_mode)viewer.single_view_uninterrupted(outframe,50);
        char c = (char)cv::waitKey(100);//delay is longer for the ease of capturing.
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
    return;
}

void LaserCalibration::image_stream_laser_ranging_calibration(CaptureFrame vid)
{
    //opening the calibration json file and reading the existing values
    read_from_json("laser_calibration_values.json","all");
    ViewFrame viewer;
    std::cout << "\rPress c to record laser center values  Press ESC to exit\n";
    
        CaptureFrame outframe = laser_ranging_single_laser(vid);//laser ranging with single laser
        if(dev_mode)viewer.single_view_uninterrupted(outframe,50);
        char c = (char)cv::waitKey(100);//delay is longer for the ease of capturing.
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
            if(dev_mode)cv::destroyWindow("Resized Image");
            calib_cancel = false;
        
        }
    
    return;
}

void LaserCalibration::image_stream_laser_ranging_calibration(CaptureFrame vid, int mode)
{
    //opening the calibration json file and reading the existing values
    read_from_json("laser_calibration_values.json","all");
    ViewFrame viewer;
    std::cout << "Calibration initiated\n";
    std::cout << "Press c to record laser center values  Press ESC to exit\n";
    
        CaptureFrame outframe = laser_ranging_single_laser(vid);//laser ranging with single laser
        char c = (char)cv::waitKey(100);//delay is longer for the ease of capturing.
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
    CaptureFrame input(image_stream,"image stream input");
    image_stream_laser_ranging_calibration(input,mode);
    return;
}

void LaserCalibration::write_to_json(std::string filename, std::string all)
{
    if(all != "all")
    {
        std::cout<<"unknown writing mode\n";
        std::cout<<"Error occured in writing to json file\n";
        return;
    }
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    std::cout<<extension<<"  "<<filename<<"\n";
    if(extension !="json")
    {
        std::cout<<"Given file is not a json file. Please provide a json file\n";
        exit(0);
    }
    try{
    std::ifstream ifs("laser_calibration_values.json");
    if(ifs.fail())throw(20);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document calibration_file;
    calibration_file.ParseStream(isw);
    rapidjson::Value &document_x_value = calibration_file["laser_center_x"];std::cout<<"lskfn\n";
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
        std::cout<<"writing to json file unsuccessful\n";
        return;
    }
    return;
}


LaserCalibration::LaserCalibration()
{
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


