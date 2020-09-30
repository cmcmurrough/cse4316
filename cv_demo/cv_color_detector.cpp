//
//    Copyright 2014 Christopher D. McMurrough
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

/*******************************************************************************************************************//**
 * @file cv_color_detector.cpp
 * @brief C++ example for identifying colors in a region of interest
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 2
#define DISPLAY_WINDOW_NAME "Camera Image"

// color constants
#define COLOR_RED 'r'
#define COLOR_GREEN 'g'
#define COLOR_BLUE 'b'
#define COLOR_ORANGE 'o'
#define COLOR_YELLOW 'y'
#define COLOR_WHITE 'w'

// declare function prototypes
bool processFrame(const cv::Mat &imageIn, cv::Mat &imageOut);
char labelColor(const cv::Mat &imageIn);
double normSqr(double x1, double y1, double z1, double x2, double y2, double z2);

/*******************************************************************************************************************//**
 * @brief Process a single image frame
 * @param[in] imageIn the input image frame
 * @param[out] imageOut the processed image frame
 * @return true if frame was processed successfully
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
bool processFrame(const cv::Mat &imageIn, cv::Mat &imageOut)
{
    // get the region of interest
    cv::Point p1(imageIn.cols / 2 - 50, imageIn.rows / 2 - 50);
    cv::Point p2(imageIn.cols / 2 + 50, imageIn.rows / 2 + 50);
    cv::Rect boundingRectangle(p1, p2);
    cv::Mat imageROI = imageIn(boundingRectangle);

    // compute the color label for the region of interest
    char color = labelColor(imageROI);

    // copy the input image frame to the ouput image
    imageIn.copyTo(imageOut);

    // annotate the output image
    switch(color)
    {
        case COLOR_RED:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(0, 0, 255), 3);
            break;
        case COLOR_GREEN:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(0, 255, 0), 3);
            break;
        case COLOR_BLUE:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(255, 0, 0), 3);
            break;
        case COLOR_YELLOW:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(0, 255, 255), 3);
            break;
        case COLOR_ORANGE:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(0, 128, 255), 3);
            break;
        case COLOR_WHITE:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(255, 255, 255), 3);
            break;
        default:
            cv::rectangle(imageOut, boundingRectangle, cv::Scalar(0, 0, 0), 3);
            break;
    }

    // return true on success
    return true;
}

/*******************************************************************************************************************//**
* @brief Process a single image frame
* @param[in] imageIn the input image region of interest
* @return the label char describing the ROI color
* @author Christopher D. McMurrough
 **********************************************************************************************************************/
char labelColor(const cv::Mat &imageIn)
{
    // split the BGR image into individual channels
    cv::Mat img_b(imageIn.rows, imageIn.cols, CV_8UC1);
    cv::Mat img_g(imageIn.rows, imageIn.cols, CV_8UC1);
    cv::Mat img_r(imageIn.rows, imageIn.cols, CV_8UC1);
    cv::Mat channels[] = {img_b, img_g, img_r};
    cv::split(imageIn, channels);

    // compute the overall intensity for each pixel as (b + g + r)
    cv::Mat intensity(imageIn.rows, imageIn.cols, CV_32F);
    cv::add(img_b, img_g, intensity);
    cv::add(intensity, img_r, intensity);
    intensity = intensity / 3.0;

    // compute the normalized color values for each channel
    cv::Mat norm_b = img_b / intensity;
    cv::Mat norm_g = img_g / intensity;
    cv::Mat norm_r = img_r / intensity;

    // compute the average normalized color value of each channel
    double avg_b = cv::mean(norm_b)[0];
    double avg_g = cv::mean(norm_g)[0];
    double avg_r = cv::mean(norm_r)[0];

    // print the color values to console
    std::printf("B: %f     G: %f     R: %f \n", avg_b, avg_g, avg_r);

    // define the reference color values
    double RED[] = {0.4, 0.5, 1.8};
    double GREEN[] = {1.0, 1.2, 1.0};
    double BLUE[] = {1.75, 1.0, 0.5};
    double YELLOW[] = {0.82, 1.7, 1.7};
    double ORANGE[] = {0.2, 1.0, 2.0};
    double WHITE[] = {2.0, 1.7, 1.7};

    // compute the squerror relative to the reference color values
    double minError = 3.0;
    double errorSqr;
    char bestFit = 'x';

    // check RED fitness
    errorSqr = normSqr(RED[0], RED[1], RED[2], avg_b, avg_g, avg_r);
    if(errorSqr < minError)
    {
        minError = errorSqr;
        bestFit = COLOR_RED;
    }
    // check GREEN fitness
    errorSqr = normSqr(GREEN[0], GREEN[1], GREEN[2], avg_b, avg_g, avg_r);
    if(errorSqr < minError)
    {
        minError = errorSqr;
        bestFit = COLOR_GREEN;
    }
    // check BLUE fitness
    errorSqr = normSqr(BLUE[0], BLUE[1], BLUE[2], avg_b, avg_g, avg_r);
    if(errorSqr < minError)
    {
        minError = errorSqr;
        bestFit = COLOR_BLUE;
    }
    // check YELLOW fitness
    errorSqr = normSqr(YELLOW[0], YELLOW[1], YELLOW[2], avg_b, avg_g, avg_r);
    if(errorSqr < minError)
    {
        minError = errorSqr;
        bestFit = COLOR_YELLOW;
    }
    // check ORANGE fitness
    errorSqr = normSqr(ORANGE[0], ORANGE[1], ORANGE[2], avg_b, avg_g, avg_r);
    if(errorSqr < minError)
    {
        minError = errorSqr;
        bestFit = COLOR_ORANGE;
    }
    // check WHITE fitness
    errorSqr = normSqr(WHITE[0], WHITE[1], WHITE[2], avg_b, avg_g, avg_r);
    if(errorSqr < minError)
    {
        minError = errorSqr;
        bestFit = COLOR_WHITE;
    }

    // return the best fit color label
    return bestFit;
}

/*******************************************************************************************************************//**
* @brief computes the squared norm of two 3-tuples
* @param[in] x1 x coordinate of the first point
* @param[in] y1 y coordinate of the first point
* @param[in] z1 z coordinate of the first point
* @param[in] x2 x coordinate of the second point
* @param[in] y2 y coordinate of the second point
* @param[in] z2 z coordinate of the second point
* @returnS the squared norm of the two tuples
* @author Christoper D. McMurrough
 **********************************************************************************************************************/
double normSqr(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2);
}

/*******************************************************************************************************************//**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
int main(int argc, char **argv)
{
    // store video capture parameters
    int cameraIndex = 0;
    bool showFrames = false;

    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <camera_index> <display_mode> \n", argv[0]);
        std::printf("WARNING: Proceeding with default execution parameters... \n");
        cameraIndex = 0;
        showFrames = true;
    }
    else
    {
        cameraIndex = atoi(argv[1]);
        showFrames = atoi(argv[2]) > 0;
    }

    // initialize the camera capture
    cv::VideoCapture capture(cameraIndex);
    if(!capture.isOpened())
    {
        std::printf("Unable to open video source, terminating program! \n");
        return 0;
    }

    // get the video source parameters
    int captureWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int captureHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    std::printf("Video source opened successfully (width=%d height=%d)! \n", captureWidth, captureHeight);

    // create the debug image windows
    if(showFrames)
    {
        cv::namedWindow(DISPLAY_WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    }

    // process data until program termination
    bool doCapture = true;
    int frameCount = 0;
    while(doCapture)
    {
        // get the start time
        double startTicks = static_cast<double>(cv::getTickCount());

        // attempt to acquire and process an image frame
        cv::Mat captureFrame;
        cv::Mat processedFrame;
        bool captureSuccess = capture.read(captureFrame);
        if(captureSuccess)
        {
            // process the image frame
            processFrame(captureFrame, processedFrame);

            // increment the frame counter
            frameCount++;
        }
        else
        {
            std::printf("Unable to acquire image frame! \n");
        }

        // update the GUI window if necessary
        if(showFrames && captureSuccess)
        {
            cv::imshow(DISPLAY_WINDOW_NAME, processedFrame);

            // check for program termination
            if(((char) cv::waitKey(1)) == 'q')
            {
                doCapture = false;
            }
        }

        // compute the frame processing time
        double endTicks = static_cast<double>(cv::getTickCount());
        double elapsedTime = (endTicks - startTicks) / cv::getTickFrequency();
        std::printf("Frame processing time: %f \n", elapsedTime);
    }

    // release program resources before returning
    capture.release();
    cv::destroyAllWindows();
}
