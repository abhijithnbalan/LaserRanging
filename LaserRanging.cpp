

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
        vid.capture_video(argv[1],"The input");
        Ranger.video_processing(vid);
    
    //---------------------------//

    // ---------IMAGE------------//
        // CaptureFrame img1,img2,img3,img4;
        // ViewFrame viewer;
        // ImageProcessing processer;
        // img1.capture_image(argv[1],"A test one");~

        // img2 = processer.hsv_segmentation(img1);
        // img3 = processer.contour_distance(img2);
        // img4 = processer.laser_ranging(img1);

        // viewer.multiple_output(img1,img2,img3,img4);
        // std::cout<<processer.get_distance()<<std::endl;
        // cv::waitKey(0);
    //----------------------------//

    return 1;
}