/*******************************************************************************************************************//**
* @file serial_polling.cpp
* @brief demonstration of safe data sharing between serial port polling and display threads
* @author Christopher D. McMurrough
***********************************************************************************************************************/

// include necessary dependencies
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <csignal>
#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 2
#define MAIN_THREAD_SLEEP_MS 100
#define POLLING_THREAD_SLEEP_MS 1

// global state variables
bool RUNNING = true;
std::mutex Mutex;

// global serial port variables
boost::asio::io_service IOService;
boost::asio::serial_port SerialPort(IOService);
std::vector<char> SerialBuffer;

// define function prototypes
void PollingThreadHandler();
void ExitHandler(int signum);

/*******************************************************************************************************************//**
* @brief program entry point
* @param[in] argc number of command line arguments
* @param[in] argv string array of command line arguments
* @return return code (0 for normal termination)
* @author Christoper D. McMurrough
***********************************************************************************************************************/
int main(int argc, char **argv)
{
    // initialize connection parameters
    std::string port_name = "COM1";
    int baud_rate = 115200;

    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <port_name> <baud_rate>\n", argv[0]);
        std::printf("Invalid command line arguments, proceeding with default values! \n");
    }
    else
    {
        port_name = argv[1];
        baud_rate = atoi(argv[2]);
    }

    // active the exit signal handler
    signal(SIGINT, ExitHandler);

    // attempt to open the serial port
    try
    {
        SerialPort.open(port_name);
        SerialPort.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        std::printf("Serial port opened successfully! \n");
    }
    catch(std::exception &e)
    {
        std::printf("Unable to open serial port, terminating program! \n");
        exit(EXIT_FAILURE);
    }

    // start the polling thread
    std::thread pollingThread(PollingThreadHandler);

    // begin processing loop
    while(RUNNING)
    {
        // check to see if we have 15 or more characters in the buffer
        Mutex.lock();
        if(SerialBuffer.size() >= 15)
        {
            // print each character
            for(int i = 0; i < SerialBuffer.size(); i++)
            {
                std::printf("%c", SerialBuffer.at(i));
            }

            // clear the buffer
            SerialBuffer.clear();
        }
        else if(SerialBuffer.size() > 0)
        {
            std::printf("Only %d characters available...\n", SerialBuffer.size());
        }
        Mutex.unlock();

        // sleep for the specified amount of time
        std::this_thread::sleep_for(std::chrono::milliseconds(MAIN_THREAD_SLEEP_MS));
    }

    // wait for the polling thread to stop
    pollingThread.join();

    // close the serial port
    std::printf("Closing serial port... \n");
    SerialPort.close();

    // terminate the program
    std::printf("Terminating program. \n");
    return 0;
}

/*******************************************************************************************************************//**
* @brief thread handler for serial port polling
* @author Christoper D. McMurrough
***********************************************************************************************************************/
void PollingThreadHandler()
{
    // poll data while processing is active
    while(RUNNING)
    {
        // read a single character from the serial port
        char c;
        boost::asio::read(SerialPort, boost::asio::buffer(&c, 1));

        // append the character to the shared receive buffer
        Mutex.lock();
        SerialBuffer.push_back(c);
        Mutex.unlock();

        // sleep for the specified amount of time
        std::this_thread::sleep_for(std::chrono::milliseconds(POLLING_THREAD_SLEEP_MS));
    }
}

/*******************************************************************************************************************//**
* @brief handler for a program exit signal (used to handle a ctrl+c keyboard event)
* @author Christoper D. McMurrough
***********************************************************************************************************************/
void ExitHandler(int signum)
{
    // set processing flag to false
    RUNNING = false;
}
