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
* @file zmq_cv_request.cpp
* @brief C++ example for receiving OpenCV image frames via a zmq request / response scheme.
* @author Christopher D. McMurrough
***********************************************************************************************************************/

#include <string>
#include <cstdio>
#include <zmq.hpp>
#include "opencv2/opencv.hpp"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Received Image"

/*******************************************************************************************************************//**
* @brief program entry point
* @param[in] argc number of command line arguments
* @param[in] argv string array of command line arguments
* @return return code (0 for normal termination)
* @author Christoper D. McMurrough
***********************************************************************************************************************/
int main(int argc, char **argv)
{
    // store display parameters
    bool showFrames = false;

    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <display_mode> \n", argv[0]);
        std::printf("WARNING: Proceeding with default execution parameters... \n");
        showFrames = true;
    }
    else
    {
        showFrames = atoi(argv[1]) > 0;
    }

    // initialize the zmq context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);

    // connect to the image server
    std::printf("Connecting to server... \n");
    socket.connect ("tcp://localhost:5555");

    // create a request object
    zmq::message_t request(5);
    memcpy(request.data(), "Hello", 5);

    // request new frames until the user presses the 'q' key
    bool requestFrames = true;
    while(requestFrames)
    {
        // send the request
        std::printf("Sending request... \n");
        socket.send(request);

        // get the reply
        zmq::message_t reply;
        socket.recv(&reply);
        std::vector<uchar> buffer;
        std::printf("Received reply: %d bytes \n", static_cast<int>(reply.size()));

        // store the reply data into an image structure
        cv::Mat image(480, 640, CV_8UC3, reply.data());

        // display the result
        if(showFrames)
        {
            cv::imshow(DISPLAY_WINDOW_NAME, image);
        }

        // check for program termination
        if(((char)) cv::waitKey(1)) == 'q')
        {
            requestFrames = false;
        }
    }

    // release program resources before returning
    socket.close();
    cv::destroyAllWindows();
    return 0;
}
