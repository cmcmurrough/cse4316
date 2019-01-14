/*******************************************************************************************************************//**
 * @file pupil_tracker.cpp
 * @brief C++ implementation of a pupil tracker based on canny edge detection
 *
 * Canny edge detector based pupil tracker accepting either live capture or read-from-file video input.
 *
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include "opencv2/opencv.hpp"
#include "PupilTracker.h"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 2
#define CAMERA_FRAME_WIDTH 640
#define CAMERA_FRAME_HEIGHT 360
#define CAMERA_FORMAT CV_8UC1
#define CAMERA_FPS 30
#define CAMERA_BRIGHTNESS 128
#define CAMERA_CONTRAST 10
#define CAMERA_SATURATION 0
#define CAMERA_HUE 0
#define CAMERA_GAIN 0
#define CAMERA_EXPOSURE -6
#define CAMERA_CONVERT_RGB false

// color constants
cv::Scalar COLOR_WHITE = CV_RGB(255, 255, 255);
cv::Scalar COLOR_RED = CV_RGB(255, 0, 0);
cv::Scalar COLOR_GREEN = CV_RGB(0, 255, 0);
cv::Scalar COLOR_BLUE = CV_RGB(0, 0, 255);
cv::Scalar COLOR_YELLOW = CV_RGB(255, 255, 0);
cv::Scalar COLOR_MAGENTA = CV_RGB(255, 0, 255);

/*******************************************************************************************************************//**
 * @brief Program entry point
 *
 * Handles image processing and display of annotated results
 *
 * @param[in] argc command line argument count
 * @param[in] argv command line argument vector
 * @returnS return status
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
int main(int argc, char** argv)
{
    // validate and parse the command line arguments
    std::string videoSource = "0";
    bool displayMode = true;
    bool flipDisplay = false;
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: <video_source> <display_mode>\n");
        std::printf("Running with default parameters... \n");
    }
    else
    {
        videoSource = argv[1];
        displayMode = atoi(argv[2]) > 0;
        flipDisplay = atoi(argv[2]) == 2;
    }

    // initialize the eye camera video capture
    cv::VideoCapture occulography;
    if((videoSource.find_first_not_of( "0123456789" ) == std::string::npos))
    {
        // video source is an integer, open as a device index
        occulography.open(std::stoi(videoSource));
    }
    else
    {
        // video source is a string, interpret as a file path
        occulography.open(videoSource);
    }

    // check to see if the video source was opened successfully
    if(!occulography.isOpened())
    {
        std::printf("Unable to initialize video source %s! \n", videoSource.c_str());
        return 0;
    }

    // set video capture parameters
    /*
    occulography.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_FRAME_WIDTH);
    occulography.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_FRAME_HEIGHT);
    occulography.set(CV_CAP_PROP_FORMAT, CAMERA_FORMAT);
    occulography.set(CV_CAP_PROP_FPS, CAMERA_FPS);
    occulography.set(CV_CAP_PROP_BRIGHTNESS, CAMERA_BRIGHTNESS);
    occulography.set(CV_CAP_PROP_CONTRAST, CAMERA_CONTRAST);
    occulography.set(CV_CAP_PROP_SATURATION, CAMERA_SATURATION);
    occulography.set(CV_CAP_PROP_HUE, CAMERA_HUE);
    occulography.set(CV_CAP_PROP_GAIN, CAMERA_GAIN);
    occulography.set(CV_CAP_PROP_EXPOSURE, CAMERA_EXPOSURE);
    occulography.set(CV_CAP_PROP_CONVERT_RGB, CAMERA_CONVERT_RGB);
    */

    // intialize the display window if necessary
    if(displayMode)
    {
        cv::namedWindow("eyeImage", cv::WINDOW_NORMAL);
        cv::setWindowProperty("eyeImage", cv::WND_PROP_FULLSCREEN, cv::WINDOW_NORMAL);
        cv::setWindowProperty("eyeImage", cv::WND_PROP_AUTOSIZE, cv::WINDOW_NORMAL);
        cv::setWindowProperty("eyeImage", cv::WND_PROP_ASPECT_RATIO, cv::WINDOW_KEEPRATIO);
    }

    // create the pupil tracking object
    PupilTracker tracker;
    tracker.setDisplay(displayMode);

    // store the frame data
    cv::Mat eyeImage;

    // store the time between frames
    int frameStartTicks, frameEndTicks, processStartTicks, processEndTicks;
    float processTime, totalTime;

    // process data until program termination
    bool isRunning = true;
    while(isRunning)
    {
        // start the timer
        frameStartTicks = clock();

        // attempt to acquire an image frame
        if(occulography.read(eyeImage))
        {
            // process the image frame
            processStartTicks = clock();
            bool trackingSuccess = tracker.findPupil(eyeImage);
            processEndTicks = clock();
            processTime = ((float)(processEndTicks - processStartTicks)) / CLOCKS_PER_SEC;

            // warn on tracking failure
            if(!trackingSuccess)
            {
                std::printf("Unable to locate pupil! \n");
            }

            // update the display
            if(displayMode)
            {
                cv::Mat displayImage(eyeImage);

                // annotate the image if tracking was successful
                if(trackingSuccess)
                {
                    // draw the pupil ellipse
                    cv::ellipse(displayImage, tracker.getEllipseRectangle(), COLOR_RED);

                    // shade the pupil area
                    cv::Mat annotation(eyeImage.rows, eyeImage.cols, CV_8UC3, 0.0);
                    cv::ellipse(annotation, tracker.getEllipseRectangle(), COLOR_MAGENTA, -1);
                    const double alpha = 0.7;
                    cv::addWeighted(displayImage, alpha, annotation, 1.0 - alpha, 0.0, displayImage);
                }

                if(flipDisplay)
                {
                    // annotate the image
                    cv::Mat displayFlipped;
                    cv::flip(displayImage, displayFlipped, 1);
                    cv::imshow("eyeImage", displayFlipped);

                    // display the annotated image
                    isRunning = cv::waitKey(1) != 'q';
                    displayFlipped.release();
                }
                else
                {
                    // display the image
                    cv::imshow("eyeImage", displayImage);
                    isRunning = cv::waitKey(1) != 'q';
                }

                // release display image
                displayImage.release();
            }
        }
        else
        {
            std::printf("WARNING: Unable to capture image from source!\n");
            occulography.set(cv::CAP_PROP_POS_FRAMES, 0);
            continue;
        }

        // stop the timer and print the elapsed time
        frameEndTicks = clock();
        totalTime = ((float)(frameEndTicks - frameStartTicks)) / CLOCKS_PER_SEC;
        std::printf("Processing time (pupil, total) (result x,y): %.4f %.4f - %.2f %.2f\n", processTime, totalTime, tracker.getEllipseCentroid().x, tracker.getEllipseCentroid().y);
    }

    // release the video source before exiting
    occulography.release();
}
