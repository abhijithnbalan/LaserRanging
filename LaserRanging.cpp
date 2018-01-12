

//Including the codes for functions
#include "capture_frame.h"
#include "view_frame.h"
#include "timer.h"
#include "image_processing.h"
#include "algorithm.h"
#include "laser_ranging.h"

int main(int argc, char** argv)//The main Function
{
    
    //----------VIDEO------------//
        LaserRanging Ranger;
        CaptureFrame vid;
        vid.capture_video(argv[1],"Final output");
        Ranger.live_laser_ranging_single_laser(vid);
    
    //---------------------------//

    // ---------IMAGE------------//
    //    LaserRanging Ranger;
    //    ViewFrame viewer;
    //    CaptureFrame img,out;
    //    Ranger.set_roi(30);
    //    img.capture_image(argv[1],"image");
    //    out = Ranger.laser_ranging_single_laser(img);
        
    // //    out = viewer.add_overlay_percent;
    // viewer.multiple_view_interrupted(out,Ranger.dehaze,Ranger.contour_overlay);
    // // viewer.multiple_view_interrupted(Ranger.ROI,Ranger.hsv_segment,Ranger.contour_overlay,Ranger.dehaze);
    // if(cv::waitKey(10)>=0)return 1;

    //----------------------------//

    // return 1;
}