
//including the necessary header files
#include "laser_ranging.h"
#include "capture_frame.h"
#include "image_processing.h"
#include "view_frame.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
//for json support -- rapidjason is used
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include <fstream>

#define PI 3.14159265

CaptureFrame LaserRanging::contour_distance(CaptureFrame object1) //Contour identification and pixel distance calculation.
{
    //necessary variable declaraiton
    cv::Mat temp, contour_overlay_frame;
    int i;//used for iteration only

    //Vectors for storing contours and their corresponding hierarchy
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    temp = object1.retrieve_image().clone();//temporary storage. this variable will be used inside this function.
    
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification"
                  << "\n";
        return object1;
    }

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Finding contours

    contour_overlay_frame = cv::Mat::zeros(temp.size(), CV_8UC3);   // this image file will be used to draw contours
    cv::Mat line_draw = cv::Mat::zeros(temp.size(), CV_8UC3); //this image file which will be used to draw line

    line_overlay_frame.reload_image(line_draw, "blank line"); //reinitialising line as blank

    for (i = 0; i < contours.size(); i++) //Loop to draw every identified contours.
    {
        drawContours(temp, contours, i, cv::Scalar(255, 0, 0), CV_FILLED); // fill the contours to eliminate hollow segments.
    }
    
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Again finding the contours(This will eliminate contors inside contours)

    //Drawing contours on the segemnted image. The contours will be stored in contour_overlay variable
    for (i = 0; i < contours.size(); i++)
    {
        cv::Rect box = boundingRect(contours[i]);
        rectangle(contour_overlay_frame, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.
    }
    
    //Distance is calculated only if two contours are identified. This will be the pixel distance.
    if (contours.size() == 2)
    {
        //Getting rectangle corresponding to the contours
        cv::Rect bounding_box1 = cv::boundingRect(contours[0]);
        cv::Rect bounding_box2 = cv::boundingRect(contours[1]);
        //Getting center of the bounding rectangle as the laser dot location
        centerx[0] = bounding_box1.x + bounding_box1.width/2;
        centerx[1] = bounding_box2.x + bounding_box2.width/2;
        centery[0] = bounding_box1.y + bounding_box1.height/2;
        centery[1] = bounding_box2.y + bounding_box2.height/2;
        //Finding distance between the points
        distance_px = sqrt(pow(centerx[0] - centerx[1], 2) + pow(centery[0] - centery[1], 2));
        //line is drawn connecting two centers.
        line(line_draw, cv::Point(centerx[0], centery[0]), cv::Point(centerx[1], centery[1]), cv::Scalar(255, 255, 255), 2, 8);
        //The line_overlay_frame is updated with the new line
        line_overlay_frame.reload_image(line_draw, "overlayed line");
    }
    else
    {
        //if distance couldn't be identified then changing the centers back to zeros
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }
    //Making a new object for returning
    CaptureFrame contour_distance(contour_overlay_frame, "Contour and distance");
    //Clearing the vectors used
    contours.clear();
    hierarchy.clear();

    return contour_distance;
}

CaptureFrame LaserRanging::contour_distance_single_laser(CaptureFrame object1) //Contour identification and pixel distance calculation.
{

    //necessary variable declaraiton
    //temp will be used as the imput image internally to this function.
    cv::Mat temp, contour_draw, line_draw;

    int i; //used only for iternation.

    //Vectors to store contours and their corresponding hierarchy
    std::vector<std::vector<cv::Point> > contours_right;
    std::vector<cv::Vec4i> hierarchy_right;
    std::vector<std::vector<cv::Point> > contours_left;
    std::vector<cv::Vec4i> hierarchy_left;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    //Initializing temp as input image
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

    //Dividing the frame but two according to the laser center as left half and right half.
    cv::Mat left_half_frame = temp(cv::Rect(0, 0, laser_center_x, temp.rows)).clone();
    cv::Mat right_half_frame = temp(cv::Rect(laser_center_x, 0, (temp.cols - laser_center_x), temp.rows)).clone();

    //Creating and Initialising variables for line for both halves
    cv::Mat line_draw_left = cv::Mat::zeros(left_half_frame.size(), CV_8UC3);
    cv::Mat line_draw_right = cv::Mat::zeros(right_half_frame.size(), CV_8UC3);
    line_overlay_left.reload_image(line_draw_left, "blank line");
    line_overlay_right.reload_image(line_draw_right, "blank line");

    //Creating and Initialising variables for contour for both halves
    cv::Mat contour_draw_left = cv::Mat::zeros(left_half_frame.size(), CV_8UC3);
    cv::Mat contour_draw_right = cv::Mat::zeros(right_half_frame.size(), CV_8UC3);
    contour_overlay_left.reload_image(contour_draw_left, "Blank contour");
    contour_overlay_right.reload_image(contour_draw_right, "Blank contour");

    //Finding contours again. This time the hollow portions would have eliminated.
    findContours(right_half_frame, contours_right, hierarchy_right, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //searching for contour in right half plane
    findContours(left_half_frame, contours_left, hierarchy_left, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);    //searching for contour in left half plane
    
    //Distance is calculated only if one contour is identified on each half image. This will be the pixel distance.
    if (contours_right.size() == 1)
    {
        cv::Rect box = boundingRect(contours_right[0]); //drawing bounding rectangle
        //identifying the center of the bounding rectangle
        centerx[0] = box.x + box.width / 2 ;
        centery[0] = box.y + box.height / 2 ;
        //Calculating the distance from laser center to the center identified
        distance_rl_px = sqrt(pow(centerx[0], 2) + pow(laser_center_y - centery[0], 2));
        //drawing the bounding rectangles.
        rectangle(contour_draw_right, box, cv::Scalar(255, 255, 255)); 
        contour_overlay_right.reload_image(contour_draw_right, "Right Half contours");
        //copying the half image contour to final contour_overlay 
        contour_draw_right.copyTo(contour_draw(cv::Rect(laser_center_x, 0, contour_draw_right.cols, contour_draw_right.rows)));
        //line is drawn connecting two centers.
        //Drawing line connecting center and laser center
        line(line_draw_right, cv::Point(0, laser_center_y - (original_frame.retrieve_image().rows / 2 - roi.height / 2)), cv::Point(centerx[0], centery[0]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_right.reload_image(line_draw_right, "overlayed line");
        //copying half line to line-ovelray
        line_draw_right.copyTo(line_draw(cv::Rect(laser_center_x, 0, line_draw_right.cols, line_draw_right.rows)));
        //converting points from half image to global(in actual image)
        centerx[0] = centerx[0] + right_half_frame.cols + roi.x;
        centery[0] = centery[0] + roi.y;
    }

    if (contours_left.size() == 1)//if one contour is identified for left half image
    {
        cv::Rect box = boundingRect(contours_left[0]);
        //taking the center of the bounding rectangle
        centerx[1] = box.x + box.width / 2 ;
        centery[1] = box.y + box.height / 2;

        //calculating the distance from the laser center to the center identified
        distance_ll_px = sqrt(pow(laser_center_x - centerx[1], 2) + pow(laser_center_y - centery[1], 2));

        //Drawing rectangle in the contour 
        rectangle(contour_draw_left, box, cv::Scalar(255, 255, 255)); 
        //updating the object with new image
        contour_overlay_left.reload_image(contour_draw_right, "Left Half contours");
        //copying to contour_draw
        contour_draw_left.copyTo(contour_draw(cv::Rect(0, 0, contour_draw_left.cols, contour_draw_left.rows)));

        //line is drawn connecting center and laser center.
        line(line_draw_left, cv::Point(laser_center_x, laser_center_y - (original_frame.retrieve_image().rows / 2 - roi.height / 2)), cv::Point(centerx[1], centery[1]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay_left.reload_image(line_draw, "overlayed line");
        //copying to actual line_draw
        line_draw_left.copyTo(line_draw(cv::Rect(0, 0, line_draw_left.cols, line_draw_left.rows)));
        //converting center from half image to global(full image)
        centerx[1] = centerx[1]+ roi.x;
        centery[1] = centery[1]+ roi.y;
    }

    //No valid laser points identified
    if (contours_left.size() != 1 && contours_right.size() != 1)
    {
        //when no centers are identified then setting the center values back to zeros
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }
    //updating the objects with final line and contour image
    contour_overlay_frame.reload_image(contour_draw, "Full Contour");
    line_overlay_frame.reload_image(line_draw, "Full line");
    //Clearing out the vectors used
    contours.clear();
    hierarchy.clear();
    contours_right.clear();
    hierarchy_right.clear();
    contours_left.clear();
    hierarchy_left.clear();

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
    timer.timer_init(); //initiating timer
    
    laser_ranging(object1,0);

    CaptureFrame output = LR_data_overlay(original_frame); //overlaying necessary data

    timer.timer_end(); //ending timer and calculating time interval and fps

    return output;
}

CaptureFrame LaserRanging::laser_ranging_single_laser(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    timer.timer_init(); //initiating timer

    laser_ranging_single_laser(object1,0);//minimal laser ranging function
    //Overlay necessary data for single laser
    CaptureFrame output = LR_data_overlay_single_laser(original_frame);

    timer.timer_end(); //ending timer and calculating time interval and fps

    return output;
}

void LaserRanging::laser_ranging_single_laser(CaptureFrame object1, int mode) //Calling every functions for laser ranging and show it on the input image.
{
    // timer.timer_init(); //initiating timer

    original_frame.reload_image(object1.retrieve_image().clone(), "copy of original");

    ROI_frame = roi_selection(original_frame);
    if (use_dehaze)//dehazing is only done when bool dehaze is set true.
    {
        dehazed_frame = algo.CLAHE_dehaze(ROI_frame);
    }
    else dehazed_frame = ROI_frame;
    //segments image according to the set color
    segmented_frame = image_segmentation(dehazed_frame);
    //identify the contours and calculate the distance
    contour_overlay_frame = contour_distance_single_laser(segmented_frame);
    //Overlay necessary data for single laser
    // CaptureFrame output = LR_data_overlay_single_laser(original_frame);

    // timer.timer_end(); //ending timer and calculating time interval and fps

    return;
}
void LaserRanging::laser_ranging(CaptureFrame object1, int mode) //Calling every functions for laser ranging and show it on the input image.
{
    // timer.timer_init(); //initiating timer

    original_frame.reload_image(object1.retrieve_image().clone(), "copy of original");

    ROI_frame = roi_selection(original_frame);
    if (use_dehaze)//dehazing is only done when bool dehaze is set true.
    {
        // std::cout<<"here\n";
        dehazed_frame = algo.CLAHE_dehaze(ROI_frame);
        
    }
    else dehazed_frame = ROI_frame;
    //segments image according to the set color
    segmented_frame = image_segmentation(ROI_frame);
    //identify the contours and calculate the distance
    contour_overlay_frame = contour_distance(segmented_frame);
    //Overlay necessary data for single laser
    // CaptureFrame output = LR_data_overlay_single_laser(original_frame);

    // timer.timer_end(); //ending timer and calculating time interval and fps

    return;
}
//laser ranging for video input
void LaserRanging::live_laser_ranging(CaptureFrame vid)
{
    if (use_dynamic_control)//Enabling control panel
    {
        //creating control panel window where all the controllers will be present
        cv::namedWindow("Control Panel", CV_WINDOW_AUTOSIZE);
        //checking for laser_ranger_status for creating laser_ranging_button 
        if(laser_range_status)cv::createTrackbar("Use LaserRanging?", "Control Panel",&laser_ranging_button_value,1,laser_ranging_button, this);
        //creating threshold selection trackbars
        cv::createTrackbar("Hue Lower threshold", "Control Panel", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "Control Panel", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Saturation Upper threshold", "Control Panel", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "Control Panel", &value_lower, 130, on_trackbar, this);
        //Creating white button
        cv::createTrackbar("Use White?","Control Panel",&white_use_value,1,on_button, this);
        cv::createTrackbar("Lightess Upper threshold", "Control Panel", &lightness_upper, 50, on_trackbar, this);
    }
    //creating two images that will only be used internally
    CaptureFrame output_frame, out_timer;
    //ViewFrame object to show output
    ViewFrame viewer;
    std::cout << "Press any key to exit "
              << "\n";
    for (;;)
    {

        vid.frame_extraction();         //Frame extraction from video
        output_frame = laser_ranging(vid); //single frame laser range detection
        if (laser_range_status)//only executed when laser_ranging is switched to true
        {
            if(use_dynamic_control)cv::destroyWindow("Resized Image");//killing previously opened window
            // pixel_distance_to_distance();

            if(dev_mode)viewer.multiple_view_uninterrupted(output_frame, segmented_frame,ROI_frame, contour_overlay_frame,70); //showing the steps as mutliple input
            printf("\r Range : %f\n",range_mm);//writing range to console
            if (cv::waitKey(3) >= 0)
                break;
        }
        else//if laser_ranging is not switched to true
        {   
            if(use_dynamic_control)cv::destroyWindow("Multiple Outputs");//closing previously opened window
            //Adding overlay indicating laser_ranging is switched off
            if(dev_mode)
            {
            if(dev_mode){CaptureFrame outframe = viewer.add_overlay_percent(vid, 5, 10, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 1.5, 2);
            viewer.single_view_uninterrupted(outframe, 50);}
            }
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}
//video laser range_mm detection with single laser ranging enabled
void LaserRanging::live_laser_ranging_single_laser(CaptureFrame vid)
{
    //calibration file is opened and laser center values are read
    std::ifstream ifs("laser_calibration_values.json");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document calibration_file;
    calibration_file.ParseStream(isw);
    laser_center_x = calibration_file["laser_center_x"].GetInt(); //Initialising the laser center for range_mm finding with single laser.
    laser_center_y = calibration_file["laser_center_y"].GetInt();
    std::cout<<"Laser center values :  x = "<<laser_center_x<<"  y = "<<laser_center_y<<"\n";

    if (use_dynamic_control)//Enabling control panel
    {
        cv::namedWindow("Control Panel", CV_WINDOW_AUTOSIZE);
        //creating this button only when laser_ranging_status is true
        if(laser_range_status)cv::createTrackbar("Use LaserRanging?", "Control Panel",&laser_ranging_button_value,1,laser_ranging_button, this);
        //trackbars for threhsold setting
        cv::createTrackbar("Hue Lower threshold", "Control Panel", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "Control Panel", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "Control Panel", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "Control Panel", &value_lower, 130, on_trackbar, this);
        //filtering white
        cv::createTrackbar("Use White?","Control Panel",&white_use_value,1,on_button, this);
        cv::createTrackbar("Lightness Upper threshold", "Control Panel", &lightness_upper, 50, on_trackbar, this);
    }
    //Creating objects which will be used internally
    CaptureFrame output_frame, out_timer, test;
    //ViewFrame object for output
    ViewFrame viewer;
    std::cout << "Press any key to exit "
              << "\n";

    for (;;)
    {
        
        vid.frame_extraction(); //frame extraction from video
        if (laser_range_status)//laser_ranging get executed only when laser_range_status is true.
        {
            if(use_dynamic_control)cv::destroyWindow("overlayed image");//destroy previously opened window
            
            output_frame = laser_ranging_single_laser(vid); //single frame laser ranging
            pixel_distance_to_distance();
            printf("\r[ Left Range : %f \tRight Range : %f ]", range_ll_mm, range_rl_mm);//printing values to console
            if(dev_mode)viewer.multiple_view_uninterrupted(output_frame, segmented_frame,ROI_frame,contour_overlay_frame,70); //showing the steps as multiple inputs
            // viewer.single_view_uninterrupted(output_frame, 50); //show output with resizing by 50 percent
            if (cv::waitKey(3) >= 0)
                break;
        }
        else
        {   //when laser_ranging is set false.
            if(use_dynamic_control)cv::destroyWindow("Multiple Outputs");//closing previously opened window.
            if(dev_mode){CaptureFrame outframe = viewer.add_overlay_percent(vid, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            viewer.single_view_uninterrupted(outframe, 50);}
            if (cv::waitKey(3) >= 0)
                return;
        }
    }
    return;
}

//Function to show data as an overlay on the output image
CaptureFrame LaserRanging::LR_data_overlay(CaptureFrame object)
{
    ViewFrame viewer;
    CaptureFrame tempo = object;
    if (centerx[1] + centerx[0] + centery[0] + centery[1] != 0)
    {
       
        tempo = viewer.add_overlay(tempo,roi.x + (centerx[0] + centerx[1]) / 2, roi.y + (centery[0] + centery[1]), distance_px);
    }

    //shwing line connecting centers and contours
    tempo = viewer.add_overlay(tempo, roi, line_overlay_frame.retrieve_image());
    tempo = viewer.add_overlay(tempo, roi, contour_overlay_frame.retrieve_image());

    CaptureFrame output;
    output = timer.add_time(tempo); //Adding time value
    output = timer.add_fps(output); //Adding time value
    return output;
}

//Function to show data on output image as an overlay with single laser range_mm enabled
CaptureFrame LaserRanging::LR_data_overlay_single_laser(CaptureFrame object)
{
    ViewFrame viewer;
    CaptureFrame tempo = object;
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

    CaptureFrame output;
    output = timer.add_time(tempo); //Adding time value
    output = timer.add_fps(output); //Adding FPS value
    return output;
}
void LaserRanging::pixel_distance_to_distance()//not complete
{
    if(distance_ll_px != 0 || distance_rl_px != 0)
    {
        range_rl_mm = parallax_constant/(2 * distance_rl_px);
        range_ll_mm = parallax_constant/(2 * distance_ll_px);
        range_mm = (range_rl_mm + range_ll_mm)/2;
    }
    else if(distance_px != 0)
    {
        range_mm = parallax_constant/(2 * distance_px);
    }
    else
    {
        range_ll_mm = range_rl_mm = range_mm = 0;
    }
    return;
}
//laser ranging for an image
void LaserRanging::image_laser_ranging_single_laser(CaptureFrame object)
{
    //using laser center value from json file
    std::ifstream ifs("laser_calibration_values.json");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document calibration_file;
    calibration_file.ParseStream(isw);
    laser_center_x = calibration_file["laser_center_x"].GetInt(); //Initialising the laser center for range_mm finding with single laser.
    laser_center_y = calibration_file["laser_center_y"].GetInt();
    std::cout<<"Laser center values :  x = "<<laser_center_x<<"  y = "<<laser_center_y<<"\n";
    
    if (use_dynamic_control)//when control panel is needed
    {
        cv::namedWindow("Multiple Outputs", CV_WINDOW_AUTOSIZE);
        //laser_ranging button is available only when laser_rainging status is false
        if(laser_range_status)cv::createTrackbar("Use LaserRanging?", "Control Panel",&laser_ranging_button_value,1,laser_ranging_button, this);
        //Trackbars for threshold adjustments
        cv::createTrackbar("Hue Lower threshold", "", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "", &value_lower, 130, on_trackbar, this);
        //when white is also need to be identified
        cv::createTrackbar("Use White?","Control Panel",&white_use_value,1,on_button, this);
        cv::createTrackbar("Lightness Upper threshold", "", &lightness_upper, 50, on_trackbar, this);
    }

    ViewFrame viewer;
    CaptureFrame in, output_frame;
    for (;;)
    {
        if(laser_range_status) //only executes when laser ranging status is true
        {
         if(use_dynamic_control)cv::destroyWindow("overlayed image" );//destroy previously created window
        output_frame = laser_ranging_single_laser(object);
        
        if(dev_mode)viewer.multiple_view_uninterrupted(output_frame, segmented_frame, ROI_frame, contour_overlay_frame,70);
        if (cv::waitKey(100) >= 0)//Lower refresh rate for image mode.
            break;
        }
        else
        {
            //no laser ranging. so direct image is given back
            if(use_dynamic_control)cv::destroyWindow("Multiple Outputs");//destroy previously created window
            CaptureFrame outframe = viewer.add_overlay_percent(object, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            if(dev_mode)viewer.single_view_uninterrupted(outframe, 50);
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}
//laser ranging for image with single laser ranging disabled
void LaserRanging::image_laser_ranging(CaptureFrame object)
{
    if (use_dynamic_control)
    {
        cv::namedWindow("Multiple Outputs", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("Use LaserRanging?", "Control Panel",&laser_ranging_button_value,1,laser_ranging_button, this);
        cv::createTrackbar("Hue Lower threshold", "", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "", &value_lower, 130, on_trackbar, this);
        //for using white also for color segmentation
        cv::createTrackbar("Use White?","Control Panel",&white_use_value,1,on_button, this);
        cv::createTrackbar("Lightness Upper threshold", "", &lightness_upper, 50, on_trackbar, this);
    }

    ViewFrame viewer;
    CaptureFrame in, output_frame;
    for (;;)
    {
        if(laser_range_status)//laser ranging happens only when laser_ranging_status is true
        {
        output_frame = laser_ranging(object);//laser ranging
        
        if(dev_mode)viewer.multiple_view_uninterrupted(output_frame, segmented_frame, ROI_frame, contour_overlay_frame,70);
        if (cv::waitKey(100) >= 0)
            break;
        }
        else//laser ranging is skipped and image is directly shown
        {
            //overlay indicating laser ranging is disabled
            if(dev_mode){CaptureFrame outframe = viewer.add_overlay_percent(object, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            viewer.single_view_uninterrupted(outframe, 50);}
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}
void LaserRanging::image_stream_laser_ranging(cv::Mat input_image)
{
    if (use_dynamic_control)
    {
        cv::namedWindow("Multiple Outputs", CV_WINDOW_AUTOSIZE);

        cv::createTrackbar("Use LaserRanging?", "Control Panel",&laser_ranging_button_value,1,laser_ranging_button, this);
        cv::createTrackbar("Hue Lower threshold", "", &hue_lower, 30, on_trackbar, this);
        cv::createTrackbar("Hue Upper threshold", "", &hue_upper, 20, on_trackbar, this);
        cv::createTrackbar("Saturation Lower threshold", "", &saturation_upper, 130, on_trackbar, this);
        cv::createTrackbar("Value Lower threshold", "", &value_lower, 130, on_trackbar, this);
        //for using white also for color segmentation
        cv::createTrackbar("Use White?","Control Panel",&white_use_value,1,on_button, this);
        cv::createTrackbar("Lightness Upper threshold", "", &lightness_upper, 50, on_trackbar, this);
    }

    ViewFrame viewer;
    CaptureFrame object(input_image,"input"), output_frame;
    for (;;)
    {
        if(laser_range_status)//laser ranging happens only when laser_ranging_status is true
        {
        output_frame = laser_ranging(object);//laser ranging
        
        if(dev_mode)viewer.multiple_view_uninterrupted(output_frame, segmented_frame,ROI_frame, contour_overlay_frame,70);
        if (cv::waitKey(100) >= 0)
            break;
        }
        else//laser ranging is skipped and image is directly shown
        {
            //overlay indicating laser ranging is disabled
            if(dev_mode){CaptureFrame outframe = viewer.add_overlay_percent(object, 20, 20, "LaserRanging : Disengaged", cv::Scalar(0, 0, 255), 0.8, 2);
            viewer.single_view_uninterrupted(outframe, 50);}
            if (cv::waitKey(3) >= 0)
                break;
        }
    }
    return;
}

void LaserRanging::image_stream_laser_ranging(cv::Mat input_image,int mode)
{

    CaptureFrame object(input_image,"input"), output_frame;

    laser_ranging(object,0);//laser ranging minimal mode for execution
    pixel_distance_to_distance();
    printf("[  Range : %f ]\n", range_mm);//printing values to console
    return;
}
void LaserRanging::image_stream_laser_ranging_single_laser(cv::Mat input_image,int mode)
{
    // ViewFrame viewer;
    CaptureFrame object(input_image,"input"), output_frame;

        laser_ranging_single_laser(object,0);//laser ranging with single laser enabled and minimal mode for execution
        // viewer.single_view_interrupted(output_frame);
        pixel_distance_to_distance();
        printf("[ Left Range : %f \tRight Range : %f ]\n", range_ll_mm, range_rl_mm);//printing values to console

    return;
}


//laser_ranging_button callback function
void LaserRanging::laser_ranging_button(int state, void *ptr)
{
    //passing this pointer to laser_ranging_handler function.
    LaserRanging *c = (LaserRanging *)(ptr);
    c->laser_ranging_handler(state);
    return;
}

void LaserRanging::laser_ranging_handler(int state)
{//parameters are changed here
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

//Funcion to output the constant length per pixel
float LaserRanging::scale_L_by_px()
{
    return float(distance_between_laser)/float(distance_px);
}

//Function to output the angle of tilt
float LaserRanging::angle_of_tilt()
{//Feature only available in single_laser mode
    if(range_ll_mm != 0 && range_rl_mm != 0)
    {
       return float(atan2(float(distance_between_laser),float(range_ll_mm - range_rl_mm))*180/PI);
    }
    else//otherwise set to zero
    {
        return 0;
    }
}



LaserRanging::LaserRanging()
{
    centerx[0] = centerx[1] = 0; //Initialising all centers and ranges to be zeros
    centery[0] = centery[1] = 0; 
    range_mm = range_ll_mm = range_rl_mm = 0;

    //For initializing the laser center point, the json file is opended and the recorded data is read.
    std::ifstream ifs("laser_calibration_values.json");//calibration filename
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document calibration_file;
    calibration_file.ParseStream(isw);
    laser_center_x = calibration_file["laser_center_x"].GetInt(); //Reading data from json
    laser_center_y = calibration_file["laser_center_y"].GetInt();
    parallax_constant = calibration_file["parallax_constant"].GetFloat();
    // calibration_distance = calibration_file["calibration_distance"].GetFloat();

    use_dehaze = false;//dehaze is not used by default (used in extreme brown water )
    use_dynamic_control = true;//dynamic control is used by default
    laser_range_status = true;laser_ranging_button_value = 1;//laser ranging is used by default
    dev_mode = false;//developer mode is disabled unless specified otherwise.
    distance_between_laser = 100;//the distance between laser pointers
    hue_lower = 20, hue_upper = 16, saturation_upper = 95, value_lower = 75, lightness_upper = 40;//A sample set of threshold vlues
}
