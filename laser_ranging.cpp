
//including the necessary header files
#include "laser_ranging.h"
#include "capture_frame.h"
#include "image_processing.h"
#include "view_frame.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#define PI 3.14159265

CaptureFrame LaserRanging::contour_distance(CaptureFrame object1) //Contour identification and pixel distance calculation.
{
    //necessary variable declaraiton
    cv::Mat temp, contour_overlay_frame;
    int i;
    //contour centers
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    temp = object1.retrieve_image().clone();
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification"
                  << "\n";
        return object1;
    }

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Finding contours

    contour_overlay_frame = cv::Mat::zeros(temp.size(), CV_8UC3);   // image file will be used to draw contours
    cv::Mat line_draw = cv::Mat::zeros(temp.size(), CV_8UC3); // image file which will be used to draw line

    line_overlay_frame.reload_image(line_draw, "blank line"); //reinitialising line as blank

    for (i = 0; i < contours.size(); i++) //Loop to draw every identified contours.
    {
        drawContours(temp, contours, i, cv::Scalar(255, 0, 0), CV_FILLED); // fill the contours to eliminate hollow segments.
    }
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Again finding the contours(This will eliminate contors inside contours)

    //Center identification of contours (4 points approximation is used)
    for (i = 0; i < contours.size(); i++)
    {
        cv::Rect box = boundingRect(contours[i]);
        rectangle(contour_overlay_frame, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.
    }
    //Distance is calculated only if two contours are identified. This will be the pixel distance.
    if (contours.size() == 2)
    {
        
        cv::Rect bounding_box1 = cv::boundingRect(contours[0]);
        cv::Rect bounding_box2 = cv::boundingRect(contours[1]);
        centerx[0] = bounding_box1.x + bounding_box1.width/2;
        centerx[1] = bounding_box2.x + bounding_box2.width/2;
        centery[0] = bounding_box1.y + bounding_box1.height/2;
        centery[1] = bounding_box2.y + bounding_box2.height/2;
        distance_px = sqrt(pow(centerx[0] - centerx[1], 2) + pow(centery[0] - centery[1], 2));
        //line is drawn connecting two centers.
        line(line_draw, cv::Point(centerx[0], centery[0]), cv::Point(centerx[1], centery[1]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_frame.reload_image(line_draw, "overlayed line");
    }
    else
    {
        //if distance couldn't be identified then changing the centers back to zeros
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }

    CaptureFrame points_distance(contour_overlay_frame, "Contour and distance");
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

    temp = object1.retrieve_image().clone();
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification"
                  << "\n";
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
        centerx[0] = box.x + box.width / 2 ;
        centery[0] = box.y + box.height / 2 ;
        //Calculating the distance from laser center to the center identified
        distance_rl_px = sqrt(pow(centerx[0], 2) + pow(laser_center_y - centery[0], 2));

        rectangle(contour_draw_right, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.
        contour_overlay_right.reload_image(contour_draw_right, "Right Half contours");

        contour_draw_right.copyTo(contour_draw(cv::Rect(laser_center_x, 0, contour_draw_right.cols, contour_draw_right.rows)));
        //line is drawn connecting two centers.

        line(line_draw_right, cv::Point(0, laser_center_y - (original_frame.retrieve_image().rows / 2 - roi.height / 2)), cv::Point(centerx[0], centery[0]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_right.reload_image(line_draw_right, "overlayed line");
        line_draw_right.copyTo(line_draw(cv::Rect(laser_center_x, 0, line_draw_right.cols, line_draw_right.rows)));
        centerx[0] = centerx[0] + right_half_frame.cols + roi.x;
        centery[0] = centery[0] + roi.y;
    }

    if (contours_left.size() == 1)
    {
        cv::Rect box = boundingRect(contours_left[0]);
        //taking the center of the bounding rectangle
        centerx[1] = box.x + box.width / 2 ;
        centery[1] = box.y + box.height / 2;

        //calculating the distance from the laser center to the center identified
        distance_ll_px = sqrt(pow(laser_center_x - centerx[1], 2) + pow(laser_center_y - centery[1], 2));

        rectangle(contour_draw_left, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.

        contour_overlay_left.reload_image(contour_draw_right, "Left Half contours");
        // std::cout<<contour_draw_left.cols<<"  "<<contour_draw_left.rows<<"\n";
        contour_draw_left.copyTo(contour_draw(cv::Rect(0, 0, contour_draw_left.cols, contour_draw_left.rows)));

        //line is drawn connecting two centers.
        line(line_draw_left, cv::Point(laser_center_x, laser_center_y - (original_frame.retrieve_image().rows / 2 - roi.height / 2)), cv::Point(centerx[1], centery[1]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_left.reload_image(line_draw, "overlayed line");
        line_draw_left.copyTo(line_draw(cv::Rect(0, 0, line_draw_left.cols, line_draw_left.rows)));
        centerx[1] = centerx[1]+ roi.x;
        centery[1] = centery[1]+ roi.y;
    }

    if (contours_left.size() != 1 && contours_right.size() != 1)
    {
        //when no centers are identified then setting the center values back to zeros
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }

    contour_overlay_frame.reload_image(contour_draw, "Full Contour");
    line_overlay_frame.reload_image(line_draw, "Full line");

    return contour_overlay_frame;
}

float LaserRanging::get_laser_distance_px() //Funtion to retrieve the laser distance data.
{
    return distance_px;
}

float LaserRanging::get_left_laser_distance_px() //Funtion to retrieve the left laser distance data.
{
    return distance_ll_px;
}

float LaserRanging::get_right_laser_distance_px() //Funtion to retrieve the right laser distance data.
{
    return distance_rl_px;
}

CaptureFrame LaserRanging::laser_ranging(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    original_frame = object1; //keeping the original_frame input

    ROI_frame = roi_selection(original_frame); //cropping out the region of interest

    if (use_dehaze)
    {
        ROI_frame = algo.CLAHE_dehaze(ROI_frame); //dehazing using CLAHE algorithm
    }
    segmented_frame = image_segmentation(ROI_frame); //color segmentation through HSV conversion

    contour_overlay_frame = contour_distance(segmented_frame); //contour identification

    CaptureFrame output = LR_data_overlay(original_frame); //overlaying necessary data

    return output;
}

CaptureFrame LaserRanging::laser_ranging_single_laser(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    timer.timer_init(); //initiating timer

    original_frame.reload_image(object1.retrieve_image().clone(), "test");
    // std::cout<<object1.window_name<<"\n";

    ROI_frame = roi_selection(original_frame);
    if (use_dehaze)
    {
        ROI_frame = algo.CLAHE_dehaze(ROI_frame);
    }
    // std::cout<<ROI_frame.window_name<<"\n";
    segmented_frame = image_segmentation(ROI_frame);
    // std::cout<<segmented_frame.window_name<<"\n";

    contour_overlay_frame = contour_distance_single_laser(segmented_frame);
    // std::cout<<contour_overlay_frame.window_name<<"\n";

    CaptureFrame output = LR_data_overlay_single_laser(original_frame);

    timer.timer_end(); //ending timer and calculating time interval and fps

    return output;
}
//laser ranging for video input
void LaserRanging::live_laser_ranging(CaptureFrame vid)
{
    if (use_dynamic_control)
    {
        cv::namedWindow("Multiple Outputs", CV_WINDOW_AUTOSIZE);
        if(laser_range_status)cv::createButton("Use LaserRanging", laser_ranging_button, this, CV_CHECKBOX, 1);
        cv::createTrackbar("Hue Lower threshold", "Control Panel", &hue_lower, 50, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "Control Panel", &hue_upper, 50, on_trackbar, this);
        cv::createTrackbar("Lightess Upper threshold", "Control Panel", &lightness_upper, 50, on_trackbar, this);
        cv::createTrackbar("Saturation Upper threshold", "Control Panel", &saturation_upper, 50, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "Control Panel", &value_lower, 50, on_trackbar, this);
    }

    CaptureFrame output_frame, out_timer;
    ViewFrame viewer;
    std::cout << "Press any key to exit "
              << "\n";
    for (;;)
    {

        vid.frame_extraction();         //Frame extraction from video
        output_frame = laser_ranging(vid); //single frame laser range_mm detection
        if (laser_range_status)
        {
            if(use_dynamic_control)cv::destroyWindow("Resized");
            // pixel_distance_to_distance();
            viewer.single_view_uninterrupted(output_frame, 50);                                       //showing the output resized to 50 percent
            viewer.multiple_view_uninterrupted(output_frame, segmented_frame, contour_overlay_frame); //showing the steps as mutliple input
            // printf("\r Range : %f\n",range_mm);
            if (cv::waitKey(3) >= 0)
                break;
        }
        else
        {
            if(use_dynamic_control)cv::destroyWindow("Multiple Outputs");
            CaptureFrame outframe = viewer.add_overlay_percent(vid, 5, 10, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 1.5, 2);
            viewer.single_view_uninterrupted(outframe, 50);
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}
//video laser range_mm detection with single laser ranging enabled
void LaserRanging::live_laser_ranging_single_laser(CaptureFrame vid)
{
    if (use_dynamic_control)
    {
        cv::namedWindow("Control Panel", CV_WINDOW_AUTOSIZE);
        if(laser_range_status)cv::createButton("Use LaserRanging", laser_ranging_button, this, CV_CHECKBOX, 1);
        cv::createTrackbar("Hue Lower threshold", "", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Lightness Upper threshold", "", &lightness_upper, 50, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "", &value_lower, 130, on_trackbar, this);
        cv::createButton("Use White?", on_button, this, CV_CHECKBOX, 0);
    }

    CaptureFrame output_frame, out_timer, test;
    ViewFrame viewer;
    std::cout << "Press any key to exit "
              << "\n";

    for (;;)
    {
        // std::cout<<cv::getNumThreads()<<"\n";
        vid.frame_extraction(); //frame extraction from video
        if (laser_range_status)
        {
            if(use_dynamic_control)cv::destroyWindow("Resized");
            
            output_frame = laser_ranging_single_laser(vid); //single frame laser ranging
            pixel_distance_to_distance();
            printf("\r[ Left Range : %f \tRight Range : %f ]", range_ll_mm, range_rl_mm);
            viewer.multiple_view_uninterrupted(output_frame, segmented_frame, contour_overlay_frame, ROI_frame); //showing the steps as multiple inputs
            // viewer.single_view_uninterrupted(output_frame, 50); //show output with resizing by 50 percent
            if (cv::waitKey(3) >= 0)
                break;
        }
        else
        {   
            if(use_dynamic_control)cv::destroyWindow("Multiple Outputs");
            CaptureFrame outframe = viewer.add_overlay_percent(vid, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            viewer.single_view_uninterrupted(outframe, 50);
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}

//Function to show data as an overlay on the output image
CaptureFrame LaserRanging::LR_data_overlay(CaptureFrame object)
{
    cv::Mat image = object.retrieve_image();
    if (centerx[1] + centerx[0] + centery[0] + centery[1] != 0)
    {
        std::ostringstream dst;
        dst << distance_px;
        std::string d(dst.str());
        //Printing distance value in the middle part of the line
        putText(image, d, cvPoint(roi.x + (centerx[0] + centerx[1]) / 2, roi.y + (centery[0] + centery[1])), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA); //optional viewing of the mark in the center of bounding box__notice the bottom right corner for centre
    }

    //shwing line connecting centers and contours
    image(roi) = image(roi).clone() - line_overlay_frame.retrieve_image();
    image(roi) = image(roi).clone() - contour_overlay_frame.retrieve_image();

    CaptureFrame output(image, "overlayed output");
    output = timer.add_time(output); //Adding maximum fps
    return object;
}

//Function to show data on output image as an overlay with single laser range_mm enabled
CaptureFrame LaserRanging::LR_data_overlay_single_laser(CaptureFrame object)
{
    ViewFrame viewer;
    CaptureFrame tempo = object;
    cv::Mat image = object.retrieve_image();
    if ((centerx[0] + centery[0]) != 0) //when left contour is identified
    {
        tempo = viewer.add_overlay(tempo, laser_center_x + centerx[0], roi.y + centery[0], distance_rl_px);
    }
    if ((centerx[1] + centery[1]) != 0) //when right contour is identified
    {
        tempo = viewer.add_overlay(tempo, centerx[1], roi.y + centery[1], distance_ll_px);
    }

    // //showing line and contour
    tempo = viewer.add_overlay(tempo, roi, line_overlay_frame.retrieve_image());
    tempo = viewer.add_overlay(tempo, roi, contour_overlay_frame.retrieve_image());

    CaptureFrame output(image, "overlayed image");
    output = timer.add_time(tempo); //Adding FPS value
    return output;
}
void LaserRanging::pixel_distance_to_distance()
{
    (centerx[0] + centery[0] != 0) ? range_ll_mm = distance_ll_px * 2 : range_ll_mm = 0;

    (centerx[1] + centery[1] != 0) ? range_rl_mm = distance_rl_px * 2 : range_rl_mm = 0;

    (centerx[1] + centery[1] != 0 && centerx[0] + centery[0] != 0) ? range_mm = range_ll_mm + range_rl_mm : range_mm = distance_px;

    return;
}

void LaserRanging::image_laser_ranging_single_laser(CaptureFrame object)
{
    if (use_dynamic_control)
    {
        cv::namedWindow("Multiple Outputs", CV_WINDOW_AUTOSIZE);

        if(laser_range_status)cv::createButton("Use LaserRanging", laser_ranging_button, this, CV_CHECKBOX, 1);
        cv::createTrackbar("Hue Lower threshold", "", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Lightness Upper threshold", "", &lightness_upper, 50, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "", &value_lower, 130, on_trackbar, this);
        cv::createButton("Use White?", on_button, this, CV_CHECKBOX, 0);
    }

    ViewFrame viewer;
    CaptureFrame in, output_frame;
    for (;;)
    {
        if(laser_range_status)
        {
         if(use_dynamic_control)cv::destroyWindow("Resized");
        output_frame = laser_ranging_single_laser(object);
        
        viewer.multiple_view_uninterrupted(output_frame, ROI_frame, segmented_frame, contour_overlay_frame);
        if (cv::waitKey(100) >= 0)
            break;
        }
        else
        {
            if(use_dynamic_control)cv::destroyWindow("Multiple Outputs");
            CaptureFrame outframe = viewer.add_overlay_percent(object, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            viewer.single_view_uninterrupted(outframe, 50);
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}

void LaserRanging::image_laser_ranging(CaptureFrame object)
{
    if (use_dynamic_control)
    {
        cv::namedWindow("Multiple Outputs", CV_WINDOW_AUTOSIZE);

        cv::createButton("Use LaserRanging", laser_ranging_button, this, CV_CHECKBOX, 1);
        cv::createTrackbar("Hue Lower threshold", "", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Lightness Upper threshold", "", &lightness_upper, 50, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "", &value_lower, 130, on_trackbar, this);
        cv::createButton("Use White?", on_button, this, CV_CHECKBOX, 0);
    }

    ViewFrame viewer;
    CaptureFrame in, output_frame;
    for (;;)
    {
        if(laser_range_status)
        {
        output_frame = laser_ranging(object);
        
        viewer.multiple_view_uninterrupted(output_frame, ROI_frame, segmented_frame, contour_overlay_frame);
        if (cv::waitKey(100) >= 0)
            break;
        }
        else
        {
            CaptureFrame outframe = viewer.add_overlay_percent(object, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            viewer.single_view_uninterrupted(outframe, 50);
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}

void LaserRanging::laser_ranging_button(int state, void *ptr)
{
    //  std::cout<<"ontrack\n";
    LaserRanging *c = (LaserRanging *)(ptr);
    c->laser_ranging_handler(state);
    return;
}

void LaserRanging::laser_ranging_handler(int state)
{
    if(state == 0)
    {
        laser_range_status = false;
    }
    else 
    {
        laser_range_status = true;    
    }
    return;
}

float LaserRanging::scale_L_by_px()
{
    return float(distance_between_laser)/float(distance_px);
}

float LaserRanging::angle_of_tilt()
{
    if(range_ll_mm != 0 && range_rl_mm != 0)
    {
       return float(atan2(float(distance_between_laser),float(range_ll_mm - range_rl_mm))*180/PI);
    }
    else
    {
        return 0;
    }
}

void LaserRanging::laser_ranging_calibraton(CaptureFrame vid)
{
    ViewFrame viewer;
    std::cout << "Calibration initialed\n";
    std::cout << "Press c to record laser center values  Press ESC to exit\n";
    for (;;)
    {
        vid.frame_extraction();
        CaptureFrame outframe = laser_ranging_single_laser(vid);
        viewer.single_view_uninterrupted(outframe,50);
        char c = (char)cv::waitKey(100);
        if (c == 99)
        {
            std::cout<<centerx[0]<<" "<<centery[0]<<"\n"<<centerx[1]<<" "<<centery[1]<<"\n";
            laser_center_x = (centerx[0] + centerx[1]) / 2;
            laser_center_y = (centery[0] + centery[1]) / 2;
            std::cout << "Capturing laser center values as  x = " << laser_center_x << " and  y = " << laser_center_y << "\n";
            // cv::destroyWindow(outframe.window_name);
            cv::destroyAllWindows();
            break;
        }
        else if (c == 27)
        {
            // cv::destroyWindow(outframe.window_name);
            cv::destroyAllWindows();
            break;
        }
    }
    return;
}

LaserRanging::LaserRanging()
{
    centerx[0] = centerx[1] = 0; //Initialising all centers to be zeros
    centery[0] = centery[1] = 0;
    range_mm = range_ll_mm = range_rl_mm = 0;

    laser_center_x = 630; //Initialising the laser center for range_mm finding with single laser.
    laser_center_y = 380;
    use_dehaze = false;
    use_dynamic_control = true;
    laser_range_status = true;
    distance_between_laser = 100;
    hue_lower = 20, hue_upper = 16, saturation_upper = 95, value_lower = 75, lightness_upper = 40;
}