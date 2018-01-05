
#include "view_frame.h"
#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>

void ViewFrame::single_view_interrupted(CaptureFrame object) //Shows a single output and wait for userkey to continue
{
    if (!object.retrieve_image().data)
    {
        if (!object.retrieve_video().isOpened())
        {
            printf("nothing to show.\n");
            return;
        }
        else
        {
            cv::Mat temporary;
            for (;;)
            {
                object.retrieve_video() >> temporary;
                cv::imshow(object.window_name, temporary);
                if (!temporary.data)
                {
                    return;
                }
                if (cv::waitKey(30) >= 0) //Waits for a key press from user to continue.
                {
                    cv::destroyWindow(object.window_name);
                    break;
                }
            }
            return;
        }
    }
    else
    {
        cv::imshow(object.window_name, object.retrieve_image());
        if (cv::waitKey(30) >= 0) //Waits for a key press from user to continue
        {
            cv::destroyWindow(object.window_name);
            return;
        }
    }
}
void ViewFrame::single_view_interrupted(CaptureFrame object, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary;
    resize(object.retrieve_image(), temporary, cv::Size(object.retrieve_image().cols * percent / 100, object.retrieve_image().rows * percent / 100));
    CaptureFrame output(temporary, "Resized Image");
    ViewFrame::single_view_interrupted(output);
    return;
}
// The following functions are uninterrupted by user input and are used in loops.
void ViewFrame::single_view_uninterrupted(CaptureFrame object) //Shows output and continues. Used inside loops.
{
    if (!object.retrieve_image().data)
    {
        if (!object.retrieve_video().isOpened())
        {
            printf("nothing to show.\n");
            cv::destroyAllWindows();
            exit(0);
        }
        else
        {
            cv::Mat temporary;
            for (;;)
            {
                object.retrieve_video() >> temporary;
                cv::imshow(object.window_name, temporary);
                return;
            }
            return;
        }
    }
    else
    {
        cv::imshow(object.window_name, object.retrieve_image());
        return;
    }
}
void ViewFrame::single_view_uninterrupted(CaptureFrame object, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary;
    resize(object.retrieve_image(), temporary, cv::Size(object.retrieve_image().cols * percent / 100, object.retrieve_image().rows * percent / 100));
    CaptureFrame output(temporary, "Resized Image");
    ViewFrame::single_view_uninterrupted(output);
    return;
}

//Multiple outputs
void ViewFrame::multiple_view_interrupted(CaptureFrame object1, CaptureFrame object2) //Shows two images side by side and waits for user input.
{
    image1 = object1.retrieve_image();
    if (image1.channels() != 3)
    {
        if (image1.channels() == 1)
        { //Convert single channel images to 3 channel for viewing
            cvtColor(image1, image1, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image2 = object2.retrieve_image();
    if (image2.channels() != 3)
    {
        if (image2.channels() == 1)
        {
            cvtColor(image2, image2, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    //Copy the images to images to a bigger image and resize it to original size
    cv::Mat full_image = cv::Mat(image1.rows, image1.cols + image2.cols, image1.type()); //the bigger image
    cv::Rect sub_roi;
    sub_roi.x = 0;
    sub_roi.y = 0;
    sub_roi.height = image1.rows;
    sub_roi.width = image1.cols;
    image1.copyTo(full_image(sub_roi));
    sub_roi.x = image1.cols;
    sub_roi.width = image2.cols;
    image2.copyTo(full_image(sub_roi));
    resize(full_image, full_image, cv::Size(full_image.cols * 0.5, full_image.rows * 0.5));
    cv::imshow("Multiple Outputs", full_image);
    if (cv::waitKey(30) >= 0) //Waits for a key press from user to continue.
    {
        cv::destroyWindow("Multiple Outputs");
        return;
    }
}
void ViewFrame::multiple_view_interrupted(CaptureFrame object1, CaptureFrame object2, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary1, temporary2;
    resize(object1.retrieve_image(), temporary1, cv::Size(object1.retrieve_image().cols * percent / 100, object1.retrieve_image().rows * percent / 100));
    resize(object2.retrieve_image(), temporary2, cv::Size(object2.retrieve_image().cols * percent / 100, object2.retrieve_image().rows * percent / 100));
    CaptureFrame output1(temporary1, "Resized Image");
    CaptureFrame output2(temporary2, "Resized Image");
    ViewFrame::multiple_view_interrupted(output1, output2);
    return;
}
void ViewFrame::multiple_view_interrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3) //Overloaded function to shows three images
{                                                                                      //Converting all single channel images to 3 channel images.
    image1 = object1.retrieve_image();
    if (image1.channels() != 3)
    {
        if (image1.channels() == 1)
        {
            cvtColor(image1, image1, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image2 = object2.retrieve_image();
    if (image2.channels() != 3)
    {
        if (image2.channels() == 1)
        {
            cvtColor(image2, image2, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image3 = object3.retrieve_image();
    if (image3.channels() != 3)
    {
        if (image3.channels() == 1)
        {
            cvtColor(image3, image3, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    //Copy Images to a bigger image and resize itto original size and shows.
    cv::Mat full_image = cv::Mat(image1.rows + image3.rows, image1.cols + image2.cols, image1.type());
    cv::Rect sub_roi;
    sub_roi.x = 0;
    sub_roi.y = 0;
    sub_roi.height = image1.rows;
    sub_roi.width = image1.cols;
    image1.copyTo(full_image(sub_roi));
    sub_roi.x = image1.cols;
    sub_roi.width = image2.cols;
    image2.copyTo(full_image(sub_roi));
    sub_roi.x = image1.cols / 2;
    sub_roi.y = image1.rows;
    sub_roi.height = image3.rows;
    sub_roi.width = image3.cols;
    image3.copyTo(full_image(sub_roi));
    resize(full_image, full_image, cv::Size(full_image.cols * 0.5, full_image.rows * 0.5));
    cv::imshow("Multiple Outputs", full_image);
    if (cv::waitKey(30) >= 0) //Waits for a key press from user to continue.
    {
        cv::destroyWindow("Multiple Outputs");
        return;
    }
}
void ViewFrame::multiple_view_interrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary1, temporary2, temporary3;
    resize(object1.retrieve_image(), temporary1, cv::Size(object1.retrieve_image().cols * percent / 100, object1.retrieve_image().rows * percent / 100));
    resize(object2.retrieve_image(), temporary2, cv::Size(object2.retrieve_image().cols * percent / 100, object2.retrieve_image().rows * percent / 100));
    resize(object3.retrieve_image(), temporary3, cv::Size(object3.retrieve_image().cols * percent / 100, object3.retrieve_image().rows * percent / 100));
    CaptureFrame output1(temporary1, "Resized Image");
    CaptureFrame output2(temporary2, "Resized Image");
    CaptureFrame output3(temporary3, "Resized Image");
    ViewFrame::multiple_view_interrupted(output1, output2, output3);
    return;
}
void ViewFrame::multiple_view_interrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3, CaptureFrame object4) //Overloaded function to show four outputs.
{                                                                                                            //Converting all single channel images to 3 channel image.
    image1 = object1.retrieve_image();
    if (image1.channels() != 3)
    {
        if (image1.channels() == 1)
        {
            cvtColor(image1, image1, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image2 = object2.retrieve_image();
    if (image2.channels() != 3)
    {
        if (image2.channels() == 1)
        {
            cvtColor(image2, image2, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image3 = object3.retrieve_image();
    if (image3.channels() != 3)
    {
        if (image3.channels() == 1)
        {
            cvtColor(image3, image3, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image4 = object4.retrieve_image();
    if (image4.channels() != 3)
    {
        if (image4.channels() == 1)
        {
            cvtColor(image4, image4, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    //Copying all images to a single image and resize it to original size.
    cv::Mat full_image = cv::Mat(image1.rows + image3.rows, image1.cols + image2.cols, image1.type());
    cv::Rect sub_roi;
    sub_roi.x = 0;
    sub_roi.y = 0;
    sub_roi.height = image1.rows;
    sub_roi.width = image1.cols;
    image1.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object1.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 0, 0), 1, CV_AA);
    sub_roi.x = image1.cols;
    sub_roi.width = image2.cols;
    image2.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object2.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 250), 1, CV_AA);
    sub_roi.y = image1.rows;
    sub_roi.height = image4.rows;
    sub_roi.width = image4.cols;
    image3.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object3.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 200, 0), 1, CV_AA);
    sub_roi.x = 0;
    sub_roi.height = image3.rows;
    sub_roi.width = image3.cols;
    image4.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object4.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
    resize(full_image, full_image, cv::Size(full_image.cols * 0.5, full_image.rows * 0.5));
    cv::imshow("Multiple Outputs", full_image);
    if (cv::waitKey(30) >= 0) //Waits for a key press from user to continue
    {
        cv::destroyWindow("Multiple Outputs");
        return;
    }
}
void ViewFrame::multiple_view_interrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3, CaptureFrame object4, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary1, temporary2, temporary3, temporary4;
    resize(object1.retrieve_image(), temporary1, cv::Size(object1.retrieve_image().cols * percent / 100, object1.retrieve_image().rows * percent / 100));
    resize(object2.retrieve_image(), temporary2, cv::Size(object2.retrieve_image().cols * percent / 100, object2.retrieve_image().rows * percent / 100));
    resize(object3.retrieve_image(), temporary3, cv::Size(object3.retrieve_image().cols * percent / 100, object3.retrieve_image().rows * percent / 100));
    resize(object4.retrieve_image(), temporary4, cv::Size(object4.retrieve_image().cols * percent / 100, object4.retrieve_image().rows * percent / 100));
    CaptureFrame output1(temporary1, "Resized Image");
    CaptureFrame output2(temporary2, "Resized Image");
    CaptureFrame output3(temporary3, "Resized Image");
    CaptureFrame output4(temporary4, "Resized Image");
    ViewFrame::multiple_view_interrupted(output1, output2, output3, output4);
    return;
}

//The following 3 functions are the overloaded functions of the above modified for usage in loops.

void ViewFrame::multiple_view_uninterrupted(CaptureFrame object1, CaptureFrame object2) //2 Images
{
    image1 = object1.retrieve_image();
    if (image1.channels() != 3)
    {
        if (image1.channels() == 1)
        {
            cvtColor(image1, image1, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image2 = object2.retrieve_image();
    if (image2.channels() != 3)
    {
        if (image2.channels() == 1)
        {
            cvtColor(image2, image2, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }

    cv::Mat full_image = cv::Mat(image1.rows, image1.cols + image2.cols, image1.type());
    cv::Rect sub_roi;
    sub_roi.x = 0;
    sub_roi.y = 0;
    sub_roi.height = image1.rows;
    sub_roi.width = image1.cols;
    image1.copyTo(full_image(sub_roi));
    sub_roi.x = image1.cols;
    sub_roi.width = image2.cols;
    image2.copyTo(full_image(sub_roi));
    resize(full_image, full_image, cv::Size(full_image.cols * 0.5, full_image.rows * 0.5));
    cv::imshow("Multiple Outputs", full_image);
    return;
}
void ViewFrame::multiple_view_uninterrupted(CaptureFrame object1, CaptureFrame object2, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary1, temporary2;
    resize(object1.retrieve_image(), temporary1, cv::Size(object1.retrieve_image().cols * percent / 100, object1.retrieve_image().rows * percent / 100));
    resize(object2.retrieve_image(), temporary2, cv::Size(object2.retrieve_image().cols * percent / 100, object2.retrieve_image().rows * percent / 100));
    CaptureFrame output1(temporary1, "Resized Image");
    CaptureFrame output2(temporary2, "Resized Image");
    ViewFrame::multiple_view_uninterrupted(output1, output2);
    return;
}
void ViewFrame::multiple_view_uninterrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3) //3 Images
{
    image1 = object1.retrieve_image();
    if (image1.channels() != 3)
    {
        if (image1.channels() == 1)
        {
            cvtColor(image1, image1, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image2 = object2.retrieve_image();
    if (image2.channels() != 3)
    {
        if (image2.channels() == 1)
        {
            cvtColor(image2, image2, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image3 = object3.retrieve_image();
    if (image3.channels() != 3)
    {
        if (image3.channels() == 1)
        {
            cvtColor(image3, image3, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }

    cv::Mat full_image = cv::Mat(image1.rows + image3.rows, image1.cols + image2.cols, image1.type());
    cv::Rect sub_roi;
    sub_roi.x = 0;
    sub_roi.y = 0;
    sub_roi.height = image1.rows;
    sub_roi.width = image1.cols;
    image1.copyTo(full_image(sub_roi));
    sub_roi.x = image1.cols;
    sub_roi.width = image2.cols;
    image2.copyTo(full_image(sub_roi));
    sub_roi.x = image1.cols / 2;
    sub_roi.y = image1.rows;
    sub_roi.height = image3.rows;
    sub_roi.width = image3.cols;
    image3.copyTo(full_image(sub_roi));
    resize(full_image, full_image, cv::Size(full_image.cols * 0.5, full_image.rows * 0.5));
    cv::imshow("Multiple Outputs", full_image);
    return;
}
void ViewFrame::multiple_view_uninterrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary1, temporary2, temporary3;
    resize(object1.retrieve_image(), temporary1, cv::Size(object1.retrieve_image().cols * percent / 100, object1.retrieve_image().rows * percent / 100));
    resize(object2.retrieve_image(), temporary2, cv::Size(object2.retrieve_image().cols * percent / 100, object2.retrieve_image().rows * percent / 100));
    resize(object3.retrieve_image(), temporary3, cv::Size(object3.retrieve_image().cols * percent / 100, object3.retrieve_image().rows * percent / 100));
    CaptureFrame output1(temporary1, "Resized Image");
    CaptureFrame output2(temporary2, "Resized Image");
    CaptureFrame output3(temporary3, "Resized Image");
    ViewFrame::multiple_view_uninterrupted(output1, output2, output3);
    return;
}
void ViewFrame::multiple_view_uninterrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3, CaptureFrame object4) //4 Images
{
    image1 = object1.retrieve_image();
    if (image1.channels() != 3)
    {
        if (image1.channels() == 1)
        {
            cvtColor(image1, image1, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image2 = object2.retrieve_image();
    if (image2.channels() != 3)
    {
        if (image2.channels() == 1)
        {
            cvtColor(image2, image2, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image3 = object3.retrieve_image();
    if (image3.channels() != 3)
    {
        if (image3.channels() == 1)
        {
            cvtColor(image3, image3, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    image4 = object4.retrieve_image();
    if (image4.channels() != 3)
    {
        if (image4.channels() == 1)
        {
            cvtColor(image4, image4, CV_GRAY2BGR);
        }
        else
        {
            std::cout << "images have different number of channels" << std::endl;
            return;
        }
    }
    cv::Mat full_image = cv::Mat(image1.rows + image3.rows, image1.cols + image2.cols, image1.type());
    cv::Rect sub_roi;
    sub_roi.x = 0;
    sub_roi.y = 0;
    sub_roi.height = image1.rows;
    sub_roi.width = image1.cols;
    image1.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object1.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 0, 0), 1, CV_AA);
    sub_roi.x = image1.cols;
    sub_roi.width = image2.cols;
    image2.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object2.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 250), 1, CV_AA);
    sub_roi.y = image1.rows;
    sub_roi.height = image4.rows;
    sub_roi.width = image4.cols;
    image3.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object3.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 200, 0), 1, CV_AA);
    sub_roi.x = 0;
    sub_roi.height = image3.rows;
    sub_roi.width = image3.cols;
    image4.copyTo(full_image(sub_roi));
    putText(full_image(sub_roi), object4.window_name, cvPoint(30, 30), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
    resize(full_image, full_image, cv::Size(full_image.cols * 0.5, full_image.rows * 0.5));
    cv::imshow("Multiple Outputs", full_image);
    return;
}
void ViewFrame::multiple_view_uninterrupted(CaptureFrame object1, CaptureFrame object2, CaptureFrame object3, CaptureFrame object4, int percent) //Overloaded function which also resizes the output
{
    cv::Mat temporary1, temporary2, temporary3, temporary4;
    resize(object1.retrieve_image(), temporary1, cv::Size(object1.retrieve_image().cols * percent / 100, object1.retrieve_image().rows * percent / 100));
    resize(object2.retrieve_image(), temporary2, cv::Size(object2.retrieve_image().cols * percent / 100, object2.retrieve_image().rows * percent / 100));
    resize(object3.retrieve_image(), temporary3, cv::Size(object3.retrieve_image().cols * percent / 100, object3.retrieve_image().rows * percent / 100));
    resize(object4.retrieve_image(), temporary4, cv::Size(object4.retrieve_image().cols * percent / 100, object4.retrieve_image().rows * percent / 100));
    CaptureFrame output1(temporary1, "Resized Image");
    CaptureFrame output2(temporary2, "Resized Image");
    CaptureFrame output3(temporary3, "Resized Image");
    CaptureFrame output4(temporary4, "Resized Image");
    ViewFrame::multiple_view_uninterrupted(output1, output2, output3, output4);
    return;
}