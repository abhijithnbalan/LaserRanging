
#include "laser_ranging.h"
#include "capture_frame.h"
#include "image_processing.h"
#include "view_frame.h"
#include <opencv2/opencv.hpp> 


CaptureFrame LaserRanging::contour_distance(CaptureFrame object1) //Contour identification and pixel distance calculation.
{
    cv::Mat temp,contour_overlay;
    int a, i;
    int *cenx = NULL;
    int *ceny = NULL;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    temp = object1.retrieve_image();
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification" << std::endl;
        return object1;
    }

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Finding contours
    contour_overlay = cv::Mat::zeros(temp.size(), CV_8UC3); // image file will be used to draw contours
    cv::Mat line_draw = cv::Mat::zeros(temp.size(), CV_8UC3);
    line_overlay.reload_image(line_draw,"blank line"); 
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
        distance = sqrt(pow(cenx[0] - cenx[1], 2) + pow(ceny[0] - ceny[1], 2));
        //line is drawn connecting two centers.
        line(line_draw, cv::Point(cenx[0], ceny[0]), cv::Point(cenx[1], ceny[1]), cv::Scalar(255, 255, 255), 2, 8);
        line_overlay.reload_image(line_draw,"overlayed line");
        centerx[0] = cenx[0];centerx[1] = cenx[1];
        centery[0] = ceny[0];centery[1] = ceny[1];
    }
    else
    {
        centerx[0] = centerx[1] = 0;
        centery[0] = centery[1] = 0;
    }
    delete[] cenx;
    delete[] ceny;
    cenx = NULL;
    ceny = NULL;
    CaptureFrame points_distance(contour_overlay, "Contour and distance");
    return points_distance;
}
float LaserRanging::get_distance() //Funtion to retrieve the distance data.
{
    return distance;
}
CaptureFrame LaserRanging::laser_ranging(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    original = object1;
    ROI = roi_selection(original);
    dehaze = algo.CLAHE_dehaze(ROI);

    hsv_segment = hsv_segmentation(dehaze);
    contour_overlay = contour_distance(hsv_segment);
    CaptureFrame output = show_overlay(original);

    return output;
}
void LaserRanging::video_processing(CaptureFrame vid)
{
    CaptureFrame out_frame, out_timer;
    ViewFrame viewer;
    ImageProcessing processer;
    std::cout << "Press any key to exit " << std::endl;
    for (;;)
    {
        timer.timer_init();
        double timer1 = (double)cv::getTickCount();
        vid.frame_extraction();
        out_frame = laser_ranging(vid);
        viewer.single_view_uninterrupted(out_frame, 50);
        viewer.multiple_view_uninterrupted(ROI,dehaze,hsv_segment,contour_overlay);
        if (cv::waitKey(1) >= 0)
            break;
        float fps = cv::getTickFrequency() / ((double)cv::getTickCount() - timer1);
        std::cout << " " << fps << std::endl;
        timer.timer_end();
    }
    return;
}
CaptureFrame LaserRanging::show_overlay(CaptureFrame object)
{
        cv::Mat image = object.retrieve_image();
        if(centerx[1]+centerx[0]+centery[0]+centery[1] != 0)
        {
            std::ostringstream dst;
            dst << distance;
            std::string d(dst.str());
            putText(image, d, cvPoint((centerx[0] + centerx[1]) / 2, (centery[0] + centery[1]) ), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA); //optional viewing of the mark in the center of bounding box__notice the bottom right corner for centre
        }
        image(roi) = image(roi).clone() - line_overlay.retrieve_image();
        image(roi) = image(roi).clone() - contour_overlay.retrieve_image();
        CaptureFrame output(image,"overlayed image");
        output = timer.add_time(output);
        return object;
}
LaserRanging::LaserRanging()
{
    centerx[0] = centerx[1] = 0;
    centery[0] = centery[1] = 0;
}