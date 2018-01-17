

//Including the codes for functions
#include "capture_frame.h"
#include "view_frame.h"
#include "timer.h"
#include "image_processing.h"
#include "algorithm.h"
#include "laser_ranging.h"
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv)//The main Function
{
LaserRanging Ranger;
// std::string filename = argv[1];
// std::string extension = filename.substr(filename.find_last_of(".") + 1);
// if(extension == "mp4" ||extension == "avi" ||extension == "flv")
// {
    // //----------VIDEO------------//
    //     CaptureFrame vid;
    //     vid.capture_video(argv[1],"Input");
    //     // Ranger.dehaze_use = true;
    //     Ranger.live_laser_ranging_single_laser(vid);
    
    // //---------------------------//
// }
// if(extension == "png" ||extension == "jpg" ||extension == "jpeg")
// {
//     // ---------IMAGE------------//
    CaptureFrame img;
    img.capture_image(argv[1],"image");
    Ranger.single_frame_laser_ranging(img);

//     //----------------------------//
// }
    return 1;
}