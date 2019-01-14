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
* @file zmq_cv_server.cpp
* @brief C++ example for acquiring and transmitting OpenCV image frames via a zmq request / response scheme.
* @author Christopher D. McMurrough
***********************************************************************************************************************/

// include necessary dependencies
#include <cstdio>
#include "opencv2/opencv.hpp"
#include <zmq.hpp>

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 2
#define DISPLAY_WINDOW_NAME "Camera Image"

// declare function prototypes
bool processFrame(const cv::Mat &imageIn, cv::Mat &imageOut);

/*******************************************************************************************************************//**
* @brief Process a single image frame
* @param[in] imageIn the input image frame
* @param[out] imageOut the processed image frame
* @return true if frame was processed successfully
* @author Christopher D. McMurrough
***********************************************************************************************************************/
bool processFrame(const cv::Mat &imageIn, cv::Mat &imageOut)
{
    // copy the input image frame to the ouput image
    imageIn.copyTo(imageOut);

    // return true on success
    return true;
}

/*******************************************************************************************************************//**
* @brief program entry point
* @param[in] argc number of command line arguments
* @param[in] argv string array of command line arguments
* @return return code (0 for normal termination)
* @author Christoper D. McMurrough
***********************************************************************************************************************/
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

    // initialize the zmq context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    // initialize the camera capture
    cv::VideoCapture capture(cameraIndex);
    if(!capture.isOpened())
    {
        std::printf("Unable to open video source, terminating program! \n");
        return 0;
    }

    // get the video source parameters
    int captureWidth = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
    int captureHeight = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    std::printf("Video source opened successfully (width=%d height=%d)! \n", captureWidth, captureHeight);

    // create the debug image windows
    if(showFrames)
    {
        cv::namedWindow(DISPLAY_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    }

    // process data until program termination
    bool doCapture = true;
    int frameCount = 0;
    while(doCapture)
    {
        // get the start time
        double startTicks = static_cast<double>(cv::getTickCount());

        // attempt to acquire an image frame
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
            if(cv::waitKey(1) == 'q')
            {
                doCapture = false;
            }
        }

        // create the message container
        zmq::message_t request;

        // poll to see if a message has arrived
        if (socket.recv(&request, ZMQ_DONTWAIT))
        {
            // process the request message
            std::printf("Received request... \n");

            // send response message if we have a successful capture
            if(captureSuccess)
            {
                size_t frameSize = captureFrame.step[0] * captureFrame.rows;
                socket.send((void*) captureFrame.data, frameSize);
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
