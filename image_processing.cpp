
//including the necessary header files
#include "image_processing.h"
#include "capture_frame.h"
#include "timer.h"
#include "view_frame.h"

#include <opencv2/opencv.hpp> 

CaptureFrame ImageProcessing::roi_selection(CaptureFrame object1) //Selecting the Region of interst. full width is taken.
{
    cv::Mat image1 = object1.retrieve_image();
    //roi selection
    roi = cv::Rect(0, image1.rows / 2 - image1.rows * roi_percentage / 200, image1.cols, image1.rows * roi_percentage / 100);
    cv::Mat temp = image1(roi).clone();
    CaptureFrame roi_image(temp, "region_of_interest");
    return roi_image;
}

//roi selection with image input
cv::Mat ImageProcessing::roi_selection(cv::Mat image1) //Selecting the Region of interst for an image file . full width is taken.
{
    roi = cv::Rect(0, image1.rows / 2 - image1.rows * roi_percentage / 200, image1.cols, image1.rows * roi_percentage / 100);
    cv::Mat output = image1(roi).clone();
    return output;
}

CaptureFrame ImageProcessing::hsv_segmentation(CaptureFrame object1) //Color segmentation. according to threshold set.
{
    cvtColor(object1.retrieve_image(), image_hsv, cv::COLOR_BGR2HSV); //Convert to HSV format for color identificati    on
    cv::Mat image_hls;
    cvtColor(object1.retrieve_image(), image_hls, cv::COLOR_BGR2HLS);
    // Segmentation according to the value set in threshold variables
    inRange(image_hsv, thresh_low_0, thresh_low_180, image_hsv_threshold_low); 
    inRange(image_hsv, thresh_high_0, thresh_high_180, image_hsv_threshold_high);
    image_hsv_threshold = image_hsv_threshold_low + image_hsv_threshold_high;

    if(white_use)
    {
        inRange(image_hls, thresh_white, cv::Scalar(255,255,255,0), image_hsv_threshold_white);
        image_hsv_threshold = image_hsv_threshold + image_hsv_threshold_white;
    }
    // imshow("hsvt",image_hsv_threshold_white);
    //Morphological Transformations for noise reduction.
    morphologyEx(image_hsv_threshold, image_hsv_threshold, cv::MORPH_OPEN, element, cv::Point(-1, -1));
    dilate(image_hsv_threshold, image_hsv_threshold, element, cv::Point(-1, -1), 3);

    CaptureFrame hsv_threshold(image_hsv_threshold, "HSV_threshold");
    return hsv_threshold;
}

cv::Mat ImageProcessing::hsv_segmentation(cv::Mat object1) //Color segmentation. according to threshold set.
{
    cvtColor(object1, image_hsv, cv::COLOR_BGR2HSV); //Convert to HSV format for color identification
    // Segmentation according to the value set in threshold variables
    inRange(image_hsv, thresh_low_0, thresh_low_180, image_hsv_threshold_low);
    inRange(image_hsv, thresh_high_0, thresh_high_180, image_hsv_threshold_high);
    image_hsv_threshold = image_hsv_threshold_low + image_hsv_threshold_high;
    //Morphological Transformations for noise reduction.
    morphologyEx(image_hsv_threshold, image_hsv_threshold, cv::MORPH_OPEN, element, cv::Point(-1, -1), 2);
    dilate(image_hsv_threshold, image_hsv_threshold, element, cv::Point(-1, -1), 3);
    return image_hsv_threshold;
}
void ImageProcessing::set_threshold(CaptureFrame object1) //Function to set the threshold value according to water type.
{
    //Conditions and set the threshold
    // //----Green---//
    //     thresh_low_0 = cv::Scalar(45, 160, 180, 0),thresh_low_180 = cv::Scalar(70, 255, 255, 0),
    //     thresh_high_0 = cv::Scalar(180, 255, 255, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0);
    // //-----Blue-----//
    //     thresh_low_0 = cv::Scalar(82, 160, 180, 0),thresh_low_180 = cv::Scalar(130, 255, 255, 0),
    //     thresh_high_0 = cv::Scalar(180, 255, 255, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0);
    // //----White---//
    //     thresh_low_0 = cv::Scalar(0, 0, 180, 0),thresh_low_180 = cv::Scalar(180, 20, 255, 0),
    //     thresh_high_0 = cv::Scalar(180, 255, 255, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0);
    // //----Yellow----//
    //     thresh_low_0 = cv::Scalar(21, 160, 180, 0),thresh_low_180 = cv::Scalar(33, 255, 255, 0),
    //     thresh_high_0 = cv::Scalar(180, 255, 255, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0);
    //------RED------//
        thresh_low_0 = cv::Scalar(0, 160, 180, 0), thresh_low_180 = cv::Scalar(16, 255, 255, 0),
        thresh_high_0 = cv::Scalar(160, 160, 180, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0);
}
void ImageProcessing::set_roi(int req_percent) // Function to set the Region of Interest
{
    roi_percentage = req_percent;
}

void ImageProcessing::on_trackbar(int red , void* ptr)
{
     std::cout<<"ontrack\n";
    ImageProcessing *c = (ImageProcessing*)(ptr);
    c->myhandler(red);
    return;
}
void ImageProcessing::on_trackbar_single(int red , void* ptr)
{
     std::cout<<"ontrack\n";
    ImageProcessing *c = (ImageProcessing*)(ptr);
    c->myhandler_single(red);
    return;
}

void ImageProcessing::myhandler(int red)
{
    thresh_low_180 = cv::Scalar(cv::getTrackbarPos("Hue Upper threshold",""),255,255,0);
    thresh_low_0 = cv::Scalar(0,255-cv::getTrackbarPos("Saturation Lower threshold",""),255-cv::getTrackbarPos("Value Lower threshold",""),0);
    thresh_high_0 = cv::Scalar(180-cv::getTrackbarPos("Hue Lower threshold",""),255-cv::getTrackbarPos("Saturation Lower threshold",""),255-cv::getTrackbarPos("Value Lower threshold",""),0);
    thresh_white = cv::Scalar(0,255-cv::getTrackbarPos("Lightness Upper threshold",""),0,0);
    std::cout<<"myhandler\n";
    std::ostringstream sst;
    if(white_use)sst<<"hl:"<<180-cv::getTrackbarPos("Hue Lower threshold","")<<" hu:"<<cv::getTrackbarPos("Hue Upper threshold","")<<" sl:"<<255-cv::getTrackbarPos("Saturation Lower threshold","")<<"vl:"<<255-cv::getTrackbarPos("Value Lower threshold","")<<" lu:"<<255-cv::getTrackbarPos("Lightness Upper threshold","");
    if(!white_use)sst<<"hl:"<<180-cv::getTrackbarPos("Hue Lower threshold","")<<" hu:"<<cv::getTrackbarPos("Hue Upper threshold","")<<" sl:"<<255-cv::getTrackbarPos("Saturation Lower threshold","")<<"vl:"<<255-cv::getTrackbarPos("Value Lower threshold","");
    std::string view = std::string(sst.str());
    cv::displayStatusBar("Multiple Outputs",view,1000);
    return;
}
void ImageProcessing::myhandler_single(int red)
{
    thresh_low_180 = cv::Scalar(cv::getTrackbarPos("Hue Upper threshold",""),255,255,0);
    thresh_low_0 = cv::Scalar(0,255-cv::getTrackbarPos("Saturation Lower threshold",""),255-cv::getTrackbarPos("Value Lower threshold",""),0);
    thresh_high_0 = cv::Scalar(180-cv::getTrackbarPos("Hue Lower threshold",""),255-cv::getTrackbarPos("Saturation Lower threshold",""),255-cv::getTrackbarPos("Value Lower threshold",""),0);
    thresh_white = cv::Scalar(0,255-cv::getTrackbarPos("Lightness Upper threshold",""),0,0);
    std::cout<<"myhandler\n";
    std::ostringstream sst;
    if(white_use)sst<<"hl:"<<180-cv::getTrackbarPos("Hue Lower threshold","")<<" hu:"<<cv::getTrackbarPos("Hue Upper threshold","")<<" sl:"<<255-cv::getTrackbarPos("Saturation Lower threshold","")<<"vl:"<<255-cv::getTrackbarPos("Value Lower threshold","")<<" lu:"<<255-cv::getTrackbarPos("Lightness Upper threshold","");
    if(!white_use)sst<<"hl:"<<180-cv::getTrackbarPos("Hue Lower threshold","")<<" hu:"<<cv::getTrackbarPos("Hue Upper threshold","")<<" sl:"<<255-cv::getTrackbarPos("Saturation Lower threshold","")<<"vl:"<<255-cv::getTrackbarPos("Value Lower threshold","");
    std::string view = std::string(sst.str());
    flag = true;
    cv::displayStatusBar("Multiple Outputs",view,1000);

    return;
}

void ImageProcessing::on_button(int state , void* ptr )
{
     std::cout<<"onbutton\n";
    ImageProcessing *c = (ImageProcessing*)(ptr);
    c->myhandlerbutton(state);
    return;
}

void ImageProcessing::myhandlerbutton(int state)
{
    if(state == 1)white_use = true;
    else white_use = false;
    std::cout<<"myhandler\n";
    return;
}

ImageProcessing::ImageProcessing() //Constructor definition The values are preset in this constructor.
{
    roi_percentage =20; //Region of interst in percentage
    thresh_low_0 = cv::Scalar(0, 140, 180, 0), thresh_low_180 = cv::Scalar(16, 255, 255, 0),
    thresh_high_0 = cv::Scalar(160, 160, 180, 0), thresh_high_180 = cv::Scalar(180, 255, 255, 0); //Threshold values preset for red color identification.
    thresh_white = cv::Scalar(0,215,0,0);
    element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(0, 0));         //Structuring element for dilation and erosion
}
