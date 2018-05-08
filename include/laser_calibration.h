#ifndef _laser_calibration_h
#define _laser_calibration_h

#include "capture_frame.h"
#include "laser_ranging.h"


#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>//opencv support
#include "logger.h"

class LaserCalibration : public LaserRanging
{
    private:
        Logger logger;
    protected:

    public:
        bool calib_trigger,calib_cancel,calibration_status;
        float calibration_distance;
        cv::Rect region_of_interes;

        void laser_ranging_calibration(CaptureFrame);
        void image_stream_laser_ranging_calibration(CaptureFrame input);
        void image_stream_laser_ranging_calibration(CaptureFrame input,int execution_mode);
        void image_stream_laser_ranging_calibration(cv::Mat input,int execution_mode);
        void write_to_json(std::string filename, std::string key, std::string value);
        void write_to_json(std::string filename, std::string all);
        CaptureFrame calibration_overlay(CaptureFrame input);
        LaserCalibration();
};
#endif