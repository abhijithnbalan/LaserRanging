
//including the necessary header files
#include "laser_ranging.h"
#include "capture_frame.h"
#include "image_processing.h"
#include "view_frame.h"
#include <opencv2/opencv.hpp>

CaptureFrame LaserRanging::contour_distance(CaptureFrame object1) //Contour identification and pixel distance calculation.
{
    //necessary variable declaraiton
    cv::Mat temp, contour_overlay;
    int a, i;
    //contour centers
    int *cenx = NULL;
    int *ceny = NULL;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    temp = object1.retrieve_image();
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification" << "\n";
        return object1;
    }

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Finding contours

    contour_overlay = cv::Mat::zeros(temp.size(), CV_8UC3);   // image file will be used to draw contours
    cv::Mat line_draw = cv::Mat::zeros(temp.size(), CV_8UC3); // image file which will be used to draw line

    line_overlay.reload_image(line_draw, "blank line"); //reinitialising line as blank

    for (i = 0; i < contours.size(); i++) //Loop to draw every identified contours.
    {
        drawContours(temp, contours, i, cv::Scalar(255, 0, 0), CV_FILLED); // fill the contours to eliminate hollow segments.
    }
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Again finding the contours(This will eliminate contors inside contours)

    //Center identification of contours (4 points approximation is used)
    cenx = new int[contours.size()];
    ceny = new int[contours.size()];
    for (i = 0; i < contours.size(); i++)
    {
        a = (contours[i].size() / 2);

        cv::Rect box = boundingRect(contours[i]);
        rectangle(contour_overlay, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.

        cenx[i] = (contours[i][0].x + contours[i][a].x + contours[i][a / 2].x + contours[i][3 * a / 2].x) / 4;

        ceny[i] = (contours[i][0].y + contours[i][a].y + contours[i][a / 2].y + contours[i][3 * a / 2].y) / 4;
    }
    //Distance is calculated only if two contours are identified. This will be the pixel distance.
    if (contours.size() == 2)
    {
        laser_distance = sqrt(pow(cenx[0] - cenx[1], 2) + pow(ceny[0] - ceny[1], 2));
        //line is drawn connecting two centers.
        line(line_draw, cv::Point(cenx[0], ceny[0]), cv::Point(cenx[1], ceny[1]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay.reload_image(line_draw, "overlayed line");
        centerx[0] = cenx[0];
        centerx[1] = cenx[1];
        centery[0] = ceny[0];
        centery[1] = ceny[1];
    }
    else
    {
        //if distance couldn't be identified then changing the centers back to zeros
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }
    //Deleting the pointers created
    delete[] cenx;
    delete[] ceny;
    cenx = NULL;
    ceny = NULL;
    CaptureFrame points_distance(contour_overlay, "Contour and distance");
    return points_distance;
}

CaptureFrame LaserRanging::contour_distance_single_laser(CaptureFrame object1) //Contour identification and pixel distance calculation.
{

    //necessary variable declaraiton
    cv::Mat temp, contour_draw, line_draw;

    int i; //used only for loops #can be negleted

    //contour centers
    std::vector<std::vector<cv::Point> > contours_right;
    std::vector<cv::Vec4i> hierarchy_right;
    std::vector<std::vector<cv::Point> > contours_left;
    std::vector<cv::Vec4i> hierarchy_left;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    temp = object1.retrieve_image();
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification" << "\n";
        return object1;
    }

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Finding contours

    contour_draw = cv::Mat::zeros(temp.size(), CV_8UC3); // image file will be used to draw contours
    line_draw = cv::Mat::zeros(temp.size(), CV_8UC3);    // image file will be used to draw line

    for (i = 0; i < contours.size(); i++) //Loop to draw every identified contours.
    {
        drawContours(temp, contours, i, cv::Scalar(255, 0, 0), CV_FILLED); // fill the contours to eliminate hollow segments.
    }

    //Dividing the frame but two according to the laser center.
    cv::Mat left_half_frame = temp(cv::Rect(0, 0, laser_center_x, temp.rows)).clone();
    cv::Mat right_half_frame = temp(cv::Rect(laser_center_x, 0, (temp.cols - laser_center_x), temp.rows)).clone();

    //Creating and Initialising variables for line
    cv::Mat line_draw_left = cv::Mat::zeros(left_half_frame.size(), CV_8UC3);
    cv::Mat line_draw_right = cv::Mat::zeros(right_half_frame.size(), CV_8UC3);
    line_overlay_left.reload_image(line_draw_left, "blank line");
    line_overlay_right.reload_image(line_draw_right, "blank line");

    //Creating and Initialising variables for contour
    cv::Mat contour_draw_left = cv::Mat::zeros(left_half_frame.size(), CV_8UC3);
    cv::Mat contour_draw_right = cv::Mat::zeros(right_half_frame.size(), CV_8UC3);
    contour_overlay_left.reload_image(contour_draw_left, "Blank contour");
    contour_overlay_right.reload_image(contour_draw_right, "Blank contour");

    findContours(right_half_frame, contours_right, hierarchy_right, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //searching for contour in right half plane
    findContours(left_half_frame, contours_left, hierarchy_left, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);    //searching for contour in left half plane

    //Distance is calculated only if two contours are identified. This will be the pixel distance.
    if (contours_right.size() == 1)
    {
        cv::Rect box = boundingRect(contours_right[0]); //drawing bounding rectangle
        //identifying the center of the bounding rectangle
        centerx[0] = box.x + box.width / 2;
        centery[0] = box.y + box.height / 2;
        //Calculating the distance from laser center to the center identified
        right_laser_distance = sqrt(pow(centerx[0], 2) + pow(laser_center_y - centery[0], 2));

        rectangle(contour_draw_right, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.
        contour_overlay_right.reload_image(contour_draw_right, "Right Half contours");

        contour_draw_right.copyTo(contour_draw(cv::Rect(laser_center_x, 0, contour_draw_right.cols, contour_draw_right.rows)));
        //line is drawn connecting two centers.

        line(line_draw_right, cv::Point(0, laser_center_y - (original.retrieve_image().rows / 2 - roi.height / 2)), cv::Point(centerx[0], centery[0]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_right.reload_image(line_draw_right, "overlayed line");
        line_draw_right.copyTo(line_draw(cv::Rect(laser_center_x, 0, line_draw_right.cols, line_draw_right.rows)));
    }

    if (contours_left.size() == 1)
    {
        cv::Rect box = boundingRect(contours_left[0]);
        //taking the center of the bounding rectangle
        centerx[1] = box.x + box.width / 2;
        centery[1] = box.y + box.height / 2;

        //calculating the distance from the laser center to the center identified
        left_laser_distance = sqrt(pow(laser_center_x - centerx[1], 2) + pow(laser_center_y - centery[1], 2));

        rectangle(contour_draw_left, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.

        contour_overlay_left.reload_image(contour_draw_right, "Left Half contours");
        // std::cout<<contour_draw_left.cols<<"  "<<contour_draw_left.rows<<"\n";
        contour_draw_left.copyTo(contour_draw(cv::Rect(0, 0, contour_draw_left.cols, contour_draw_left.rows)));

        //line is drawn connecting two centers.
        line(line_draw_left, cv::Point(laser_center_x, laser_center_y - (original.retrieve_image().rows / 2 - roi.height / 2)), cv::Point(centerx[1], centery[1]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_left.reload_image(line_draw, "overlayed line");
        line_draw_left.copyTo(line_draw(cv::Rect(0, 0, line_draw_left.cols, line_draw_left.rows)));
    }

    if (contours_left.size() != 1 && contours_right.size() != 1)
    {
        //when no centers are identified then setting the center values back to zeros
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }

    contour_overlay.reload_image(contour_draw, "Full Contour");
    line_overlay.reload_image(line_draw, "Full line");

    return contour_overlay;
}

float LaserRanging::get_laser_distance() //Funtion to retrieve the laser distance data.
{
    return laser_distance;
}

float LaserRanging::get_left_laser_distance() //Funtion to retrieve the left laser distance data.
{
    return left_laser_distance;
}

float LaserRanging::get_right_laser_distance() //Funtion to retrieve the right laser distance data.
{
    return right_laser_distance;
}

CaptureFrame LaserRanging::laser_ranging(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    original = object1; //keeping the original input

    ROI = roi_selection(original); //cropping out the region of interest

    // dehaze = algo.CLAHE_dehaze(ROI);//dehazing using CLAHE algorithm

    hsv_segment = hsv_segmentation(ROI); //color segmentation through HSV conversion

    contour_overlay = contour_distance(hsv_segment); //contour identification

    CaptureFrame output = show_overlay(original); //overlaying necessary data

    return output;
}

CaptureFrame LaserRanging::laser_ranging_single_laser(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    timer.timer_init(); //initiating timer

    original = object1;
    ROI = roi_selection(original);

    dehaze = algo.CLAHE_dehaze(ROI);

    hsv_segment = hsv_segmentation(dehaze);

    contour_overlay = contour_distance_single_laser(hsv_segment);

    CaptureFrame output = show_overlay_single_laser(original);

    timer.timer_end(); //ending timer and calculating time interval and fps

    return output;
}
//laser ranging for video input
void LaserRanging::live_laser_ranging(CaptureFrame vid)
{
    CaptureFrame out_frame, out_timer;
    ViewFrame viewer;
    std::cout << "Press any key to exit " << "\n";
    for (;;)
    {

        vid.frame_extraction();                          //Frame extraction from video
        out_frame = laser_ranging(vid);                  //single frame laser range detection
        pixel_distance_to_distance();
        // viewer.single_view_uninterrupted(out_frame, 50); //showing the output resized to 50 percent
        viewer.multiple_view_uninterrupted(out_frame,dehaze,hsv_segment,contour_overlay);//showing the steps as mutliple input
        printf("\r Range : %f",range);
        if (cv::waitKey(1) >= 0)
            break;
    }
    return;
}
//video laser range detection with single laser ranging enabled
void LaserRanging::live_laser_ranging_single_laser(CaptureFrame vid)
{
    CaptureFrame out_frame, out_timer, test;
    ViewFrame viewer;
    std::cout << "Press any key to exit " << "\n";

    for (;;)
    {

        vid.frame_extraction();                      //frame extraction from video
        out_frame = laser_ranging_single_laser(vid); //single frame laser ranging
        pixel_distance_to_distance();
        printf("\r[ Left Range : %f \tRight Range : %f ]",left_range,right_range);
        viewer.multiple_view_uninterrupted(out_frame,hsv_segment,contour_overlay,ROI);//showing the steps as multiple inputs
        // viewer.single_view_uninterrupted(out_frame, 50); //show output with resizing by 50 percent
        if (cv::waitKey(2) >= 0)
            break;
    }
    return;
}
//Function to show data as an overlay on the output image
CaptureFrame LaserRanging::show_overlay(CaptureFrame object)
{
    cv::Mat image = object.retrieve_image();
    if (centerx[1] + centerx[0] + centery[0] + centery[1] != 0)
    {
        std::ostringstream dst;
        dst << laser_distance;
        std::string d(dst.str());
        //Printing distance value in the middle part of the line
        putText(image, d, cvPoint(roi.x + (centerx[0] + centerx[1]) / 2, roi.y + (centery[0] + centery[1])), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA); //optional viewing of the mark in the center of bounding box__notice the bottom right corner for centre
    }

    //shwing line connecting centers and contours
    image(roi) = image(roi).clone() - line_overlay.retrieve_image();
    image(roi) = image(roi).clone() - contour_overlay.retrieve_image();

    CaptureFrame output(image, "overlayed image");
    output = timer.add_fps(output); //Adding maximum fps
    return object;
}

//Function to show data on output image as an overlay with single laser range enabled
CaptureFrame LaserRanging::show_overlay_single_laser(CaptureFrame object)
{
    ViewFrame viewer;
    CaptureFrame tempo = object;
    cv::Mat image = object.retrieve_image();
    if ((centerx[0] + centery[0]) != 0) //when left contour is identified
    {
        tempo = viewer.add_overlay(tempo, laser_center_x + centerx[0], roi.y + centery[0], right_laser_distance);
    }
    if ((centerx[1] + centery[1]) != 0) //when right contour is identified
    {
        tempo = viewer.add_overlay(tempo, centerx[1], roi.y + centery[1], left_laser_distance);
    }

    // //showing line and contour
    tempo = viewer.add_overlay(tempo, roi, line_overlay.retrieve_image());
    tempo = viewer.add_overlay(tempo, roi, contour_overlay.retrieve_image());

    CaptureFrame output(image, "overlayed image");
    output = timer.add_time(tempo); //Adding FPS value
    return output;
}
void LaserRanging::pixel_distance_to_distance()
{
    (centerx[0]+centery[0] != 0)?left_range = left_laser_distance*2 : left_range = 0;
  

    (centerx[1]+centery[1] != 0)?right_range = right_laser_distance*2 : right_range = 0;
  
    (centerx[1]+centery[1] != 0 && centerx[0]+centery[0] != 0)?range = left_range + right_range : range = laser_distance;
    
    return;
}

LaserRanging::LaserRanging()
{
    centerx[0] = centerx[1] = 0; //Initialising all centers to be zeros
    centery[0] = centery[1] = 0;
    range = left_range = right_range = 0;

    laser_center_x = 630; //Initialising the laser center for range finding with single laser.
    laser_center_y = 380;
}