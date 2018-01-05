
#include "image_processing.h"
#include "capture_frame.h"
#include "timer.h"
#include "view_frame.h"

#include <opencv2/opencv.hpp> 


CaptureFrame ImageProcessing::roi_selection(CaptureFrame object1) //Selecting the Region of interst. full width is taken.
{
    cv::Mat image1 = object1.retrieve_image();
    roi = cv::Rect(0, image1.rows / 2 - image1.rows * roi_percentage / 200, image1.cols, image1.rows * roi_percentage / 100);
    cv::Mat temp = image1(roi).clone();
    CaptureFrame roi_image(temp, "region_of_interest");
    return roi_image;
}
CaptureFrame ImageProcessing::hsv_segmentation(CaptureFrame object1) //Color segmentation. according to threshold set.
{
    cvtColor(object1.retrieve_image(), image_hsv, cv::COLOR_BGR2HSV); //Convert to HSV format for color identification
    // Segmentation according to the value set in threshold variables
    inRange(image_hsv, thresh_low_0, thresh_low_180, image_hsv_threshold_low);
    inRange(image_hsv, thresh_high_0, thresh_high_180, image_hsv_threshold_high);
    image_hsv_threshold = image_hsv_threshold_low + image_hsv_threshold_high;
    //Morphological Transformations for noise reduction.
    morphologyEx(image_hsv_threshold, image_hsv_threshold, cv::MORPH_OPEN, element, cv::Point(-1, -1), 2);
    dilate(image_hsv_threshold, image_hsv_threshold, element, cv::Point(-1, -1), 3);

    CaptureFrame hsv_threshold(image_hsv_threshold, "HSV_threshold");
    return hsv_threshold;
}
CaptureFrame ImageProcessing::contour_distance(CaptureFrame object1) //Contour identification and pixel distance calculation.
{
    cv::Mat temp;
    int a, i;
    int *cenx = NULL;
    int *ceny = NULL;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat drawing;
    temp = object1.retrieve_image();
    if (temp.channels() != 1) //check whether the image is segemented or not.
    {
        std::cout << "The image is not segemented for contour identification" << std::endl;
        return object1;
    }

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Finding contours
    drawing = cv::Mat::zeros(temp.size(), CV_8UC3);                                 // image file will be used to draw contours

    for (i = 0; i < contours.size(); i++) //Loop to draw every identified contours.
    {
        drawContours(temp, contours, i, cv::Scalar(255, 0, 0), CV_FILLED); // fill the contours to eliminate hollow segments.
    }
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //Again finding the contours(This will eliminate contors inside contours)
    // imshow("tempo",temp);
    //Center identification of contours (4 points approximation is used)
    cenx = new int[contours.size()];
    ceny = new int[contours.size()];
    for (i = 0; i < contours.size(); i++)
    {
        a = (contours[i].size() / 2);

        cv::Rect box = boundingRect(contours[i]);
        rectangle(drawing, box, cv::Scalar(255, 255, 255)); //center of rectangle also can be used.

        cenx[i] = (contours[i][0].x + contours[i][a].x + contours[i][a / 2].x + contours[i][3 * a / 2].x) / 4;

        ceny[i] = (contours[i][0].y + contours[i][a].y + contours[i][a / 2].y + contours[i][3 * a / 2].y) / 4;
    }
    //Distance is calculated only if two contours are identified. This will be the pixel distance.
    if (contours.size() == 2)
    {
        distance = sqrt(pow(cenx[0] - cenx[1], 2) + pow(ceny[0] - ceny[1], 2));
        //line is drawn connecting two centers.
        line(drawing, cv::Point(cenx[0], ceny[0]), cv::Point(cenx[1], ceny[1]), cv::Scalar(255, 255, 255), 2, 8);
        //The distance is shown above the line
        std::ostringstream dst;
        dst << distance;
        std::string d(dst.str());
        putText(drawing, d, cvPoint((cenx[0] + cenx[1]) / 2, (ceny[0] + ceny[1]) / 2), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA); //optional viewing of the mark in the center of bounding box__notice the bottom right corner for centre
    }
    delete[] cenx;
    delete[] ceny;
    cenx = NULL;
    ceny = NULL;
    CaptureFrame points_distance(drawing, "Contour and distance");
    return points_distance;
}
float ImageProcessing::get_distance() //Funtion to retrieve the distance data.
{
    return distance;
}
CaptureFrame ImageProcessing::laser_ranging(CaptureFrame object1) //Calling every functions for laser ranging and show it on the input image.
{
    cv::Mat temp;
    temp = object1.retrieve_image().clone();
    CaptureFrame output = contour_distance(hsv_segmentation(CLAHE_dehaze(roi_selection(object1))));
    temp(roi) = temp(roi).clone() - output.retrieve_image();
    CaptureFrame laser_distance(temp, "Image_with_distance");
    return laser_distance;
}
void ImageProcessing::set_threshold(CaptureFrame object1) //Function to set the threshold value according to water type.
{
    //Conditions and set the threshold
    thresh_low_0 = cv::Scalar(0, 160, 180, 0), thresh_low_180 = cv::Scalar(16, 255, 255, 0),
    thresh_high_0 = cv::Scalar(160, 160, 180, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0);
}
void ImageProcessing::set_roi(int req_percent) // Function to set the Region of Interest
{
    roi_percentage = req_percent;
}
CaptureFrame ImageProcessing::CLAHE_dehaze(CaptureFrame object) //CLAHE based basic dehazing algorithm
{

    cvtColor(object.retrieve_image(), image_hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    split(image_hsv, channels);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2);
    clahe->apply(channels[2], channels[2]);
    clahe->apply(channels[1], channels[1]);
    channels[2] = channels[2] * 0.85;
    merge(channels, image_hsv);
    cv::Mat dehazed;
    cvtColor(image_hsv, dehazed, cv::COLOR_HSV2BGR);
    GaussianBlur(dehazed, dehazed, cv::Size(3, 3), 2, 2);
    CaptureFrame output(dehazed, "Dehazed(CLAHE) image");
    return output;
}

void ImageProcessing::video_processing(CaptureFrame vid)
{
    CaptureFrame out_frame, out_timer, dehaze;
    ViewFrame viewer;
    ImageProcessing processer;
    Timer timer;
    std::cout << "Press any key to exit " << std::endl;
    for (;;)
    {
        timer.timer_init();
        double timer1 = (double)cv::getTickCount();
        vid.frame_extraction();
        out_frame = processer.laser_ranging(vid);

        out_timer = timer.add_time(out_frame);
        viewer.single_view_uninterrupted(out_frame, 50);
        if (cv::waitKey(1) >= 0)
            break;
        float fps = cv::getTickFrequency() / ((double)cv::getTickCount() - timer1);
        std::cout << "                                             " << fps << std::endl;
        timer.timer_end();
    }
    return;
}
ImageProcessing::ImageProcessing() //Constructor definition The values are preset in this constructor.
{
    roi_percentage = 30; //Region of interst in percentage
    thresh_low_0 = cv::Scalar(0, 140, 180, 0), thresh_low_180 = cv::Scalar(16, 255, 255, 0),
    thresh_high_0 = cv::Scalar(160, 140, 180, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0); //Threshold values preset for red color identification.
    element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(0, 0));         //Structuring element for dilation and erosion
}
