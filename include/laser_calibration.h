#ifndef _laser_calibration_h
#define _laser_calibration_h

#include "capture_frame.h"
#include "laser_ranging.h"


#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>//opencv support

class LaserCalibration : public LaserRanging
{
    private:

    protected:

    public:
        bool calib_trigger,calib_cancel,calibration_status;
        float calibration_distance;

        void laser_ranging_calibration(CaptureFrame);
        void image_stream_laser_ranging_calibration(CaptureFrame input);
        void image_stream_laser_ranging_calibration(CaptureFrame input,int execution_mode);
        void image_stream_laser_ranging_calibration(cv::Mat input,int execution_mode);
        void write_to_json(std::string filename, std::string key, std::string value);
        void write_to_json(std::string filename, std::string all);
        LaserCalibration();
};
#endif