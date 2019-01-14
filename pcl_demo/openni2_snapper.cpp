/***********************************************************************************************************************
 * @file openni2_snapper.cpp
 * @brief Template for acquiring PCL point clouds from an OpenNI2 device
 *
 * Template for acquiring PCL point clouds from an OpenNI2 device. Incoming data streams from an OpenNI2 compliant
 * device are acquired and converted to PCL point clouds, which are then visualized in real time.
 *
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

#include <pcl/io/openni2_grabber.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>
#include <pcl/common/time.h>
#include <pcl/io/pcd_io.h>

#define NUM_COMMAND_ARGS 2

using namespace std;

/***********************************************************************************************************************
 * @class OpenNI2Processor
 * @brief Class containing data acquisition mechanics for OpenNI2 devices
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
class OpenNI2Processor
{
private:

    // store the display and save settings for the session
    int m_cloudRenderSetting;
    int m_cloudSaveSetting;

    // create a stop watch for measuring time
    pcl::StopWatch m_stopWatch;

    // create the cloud viewer object
    pcl::visualization::CloudViewer m_viewer;

public:

    /***********************************************************************************************************************
     * @brief Class constructor
     * @param[in] cloudRenderSetting sets the cloud visualization mode (render_off:0, render_on:1)
     * @param[in] cloudSaveSetting sets the disk save mode for cloud data (saves_off:0, saves_on:1)
     * @author Christopher D. McMurrough
     **********************************************************************************************************************/
    OpenNI2Processor(int cloudRenderSetting, int cloudSaveSetting) : m_viewer("Rendering Window")
    {
        // store the render and save settings
        m_cloudRenderSetting = cloudRenderSetting;
        m_cloudSaveSetting = cloudSaveSetting;

        // if the render setting is 0, force the visualization window to close
        if(m_cloudRenderSetting == 0)
        {
            m_viewer.~CloudViewer();
            std::printf("Running with visualization OFF... \n");
        }
    }

    /***********************************************************************************************************************
     * @brief Starts data acquisition and handling
     * @author Christopher D. McMurrough
     **********************************************************************************************************************/
    void run()
    {
        // create a new grabber for OpenNI2 devices
        pcl::Grabber* interface = new pcl::io::OpenNI2Grabber();

        // bind the callbacks to the appropriate member functions
        boost::function<void (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> f = boost::bind(&OpenNI2Processor::cloudCallback, this, _1);

        // connect callback function for desired signal. In this case its a point cloud with color values
        interface->registerCallback(f);

        // start receiving point clouds
        interface->start();

        // start the timer
        m_stopWatch.reset();

        // wait until user quits program
        while (!m_viewer.wasStopped())
        {
            //m_viewer.spinOnce();
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
        }

        // stop the grabber
        interface->stop();
    }

    /***********************************************************************************************************************
     * @brief Callback function for received cloud data
     * @param[in] cloudIn the raw cloud data received by the OpenNI2 device
     * @author Christopher D. McMurrough
     **********************************************************************************************************************/
    void cloudCallback(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloudIn)
    {
        // get the elapsed time since the last callback
        double elapsedTime = m_stopWatch.getTimeSeconds();
        m_stopWatch.reset();
        std::printf("Seconds elapsed since last cloud callback: %f \n", elapsedTime);

        // store the cloud save count
        static int saveCount = 0;

        // render cloud if necessary
        if(m_cloudRenderSetting)
        {
            m_viewer.showCloud(cloudIn);
        }

        // save the cloud if necessary
        if(m_cloudSaveSetting)
        {
            std::stringstream ss;
            string str;
            ss << saveCount << ".pcd";
            str = ss.str();
            pcl::io::savePCDFile<pcl::PointXYZRGBA> (str.c_str(), *cloudIn, true);
            std::printf("cloud saved to %s\n", str.c_str());
            saveCount++;
        }
    }
};

/***********************************************************************************************************************
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @returnS return code (0 for normal termination)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
int main (int argc, char** argv)
{
    // store the run time settings
    int cloudRenderSetting;
    int cloudSaveSetting;

    // parse and validate the command line arguments
    if(argc == 1)
    {
        // assign default settings if no arguments were given
        cloudRenderSetting = 1;
        cloudSaveSetting = 0;
    }
    else if(argc != NUM_COMMAND_ARGS + 1)
    {
        // return if we do not have the proper amount of arguments
        std::printf("USAGE: %s <cloud_render_setting> <cloud_save_setting> \n", argv[0]);
        return 0;
    }
    else
    {
        // parse the command line arguments
        cloudRenderSetting = atoi(argv[1]);
        cloudSaveSetting = atoi(argv[2]);
    }

    // create the processing object
    OpenNI2Processor ONI2Processor(cloudRenderSetting, cloudSaveSetting);

    // start the processing object
    ONI2Processor.run();

    // exit program
    return 0;
}
