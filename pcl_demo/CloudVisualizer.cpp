/***********************************************************************************************************************
 * @file CloudVisualizer.h
 * @brief Implementation of the CloudVisualizer wrapper class
 *
 * This class provides a wrapper for PCL visualization functions
 *
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/

#include "CloudVisualizer.h"

#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/octree/octree.h>
#include <Eigen/Core>

using namespace std;

/***********************************************************************************************************************
 * @brief Class constructor
 *
 * Initializes the CloudVisualizer class by creating a rendering window with the given name
 *
 * @param[in] windowName name of the rendering window (default: "")
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
CloudVisualizer::CloudVisualizer(const string &windowName)
{
    myViewer.reset(new pcl::visualization::PCLVisualizer(windowName));
    myViewer->initCameraParameters();
    myViewer->setBackgroundColor(0, 0, 0);
}

/***********************************************************************************************************************
 * @brief Perform one interation of rendering
 *
 * Performs a single iteration of rendering and event checking with a maximum execution time
 *
 * @param[in] maxTime the time allowed for rendering and event handling, in ms
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::spin(int maxTimeMs)
{
    myViewer->spinOnce(maxTimeMs);
}

/***********************************************************************************************************************
 * @brief Check to see if the visualization window is running
 *
 * Polls the state of the visualization
 *
 * @return true if the window is running
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
bool CloudVisualizer::isRunning()
{
    return !myViewer->wasStopped();
}

/***********************************************************************************************************************
 * @brief Register a UI window point picking callback
 *
 * binds a callback function to a raised point picking event
 *
 * @param[in] callback reference to the callback handling function
 * @param[in] cloud pointer to the rendered cloud that causes the callback
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::registerPointPickingCallback(void (*callback) (const pcl::visualization::PointPickingEvent&, void*), pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloud)
{
    myViewer->registerPointPickingCallback(callback, static_cast<void*>(&cloud));
}

/***********************************************************************************************************************
 * @brief Register a UI window keyboard callback
 *
 * binds a callback function to a raised keyboard event
 *
 * @param[in] callback reference to the callback handling function
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::registerKeyboardCallback(void (*callback) (const pcl::visualization::KeyboardEvent&, void*))
{
    myViewer->registerKeyboardCallback(callback, (void*) &myViewer);
}

/***********************************************************************************************************************
 * @brief Add a cloud to the rendering window
 *
 * Adds a point cloud to the rendering window. The size, id, and view port can optionally be set.
 *
 * @param[in] cloud the point cloud to be added to the
 * @param[in] pointSize the display size of the individual cloud points (default: 1.0)
 * @param[in] id the unique identifier of the input cloud (default: "cloud")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addCloud(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud, double pointSize, const string &id, int viewPort)
{
    // add a cloud to the display
    pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGBA> rgb(cloud);
    myViewer->addPointCloud<pcl::PointXYZRGBA>(cloud, rgb, id);
    myViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, pointSize, id);
}

/***********************************************************************************************************************
 * @brief Add a cloud to the rendering window
 *
 * Updates the data of a given rendered point cloud
 *
 * @param[in] cloud the point cloud to be added to the
 * @param[in] id the unique identifier of the input cloud (default: "cloud")
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::updateCloud(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud, const string &id)
{
    myViewer->updatePointCloud(cloud, id);
}

/***********************************************************************************************************************
 * @brief Add a coordinate frame to the display
 *
 * Adds a coordinate frame to the rendering window. position, orientation, and scale can be configured.
 *
 * @param[in] position the translation of the frame relative to the origin
 * @param[in] orientation the quaternion orientation of the frame relative to the origin
 * @param[in] scale the size of the coordinate frame axes (default: 1.0)
 * @param[in] id the unique identifier of the coordinate frame (default: "frame")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addCoordinateFrame(const Eigen::Vector4f &position, const Eigen::Quaternionf &orientation, double scale, const string &id, int viewPort)
{
    // convert the quaternion to an affine transformation
    Eigen::Matrix3f rotation = orientation.normalized().toRotationMatrix();
    Eigen::Affine3f t;
    t(0,0) = rotation(0,0);
    t(0,1) = rotation(0,1);
    t(0,2) = rotation(0,2);
    t(0,3) = position[0];
    t(1,0) = rotation(1,0);
    t(1,1) = rotation(1,1);
    t(1,2) = rotation(1,2);
    t(1,3) = position[1];
    t(2,0) = rotation(2,0);
    t(2,1) = rotation(2,1);
    t(2,2) = rotation(2,2);
    t(2,3) = position[2];
    t(3,0) = 0;
    t(3,1) = 0;
    t(3,2) = 0;
    t(3,3) = 1;


    // add the coordinate frame to the display
    myViewer->addCoordinateSystem(scale, t, id, viewPort);
}

/***********************************************************************************************************************
 * @brief Add a coordinate frame to the display
 *
 * Adds a coordinate frame to the rendering window. position, orientation, and scale can be configured.
 *
 * @param[in] x the x coordinate of the frame origin
 * @param[in] y the y coordinate of the frame origin
 * @param[in] z the z coordinate of the frame origin
 * @param[in] roll the roll coordinate of the frame origin
 * @param[in] pitch the pitch coordinate of the frame origin
 * @param[in] yaw the yaw coordinate of the frame origin
 * @param[in] pointSize the display size of the individual cloud points (default: 1.0)
 * @param[in] scale the size of the coordinate frame axes (default: 1.0)
 * @param[in] id the unique identifier of the coordinate frame (default: "frame")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addCoordinateFrame(double x, double y, double z, double roll, double pitch, double yaw, double scale, const string &id, int viewPort)
{
    // convert the roll, pitch, yaw angles to an affine transformation
    Eigen::Affine3f transformation = pcl::getTransformation((float) x, (float) y, (float) z, (float) roll, (float) pitch, (float) yaw);

    // add the coordinate frame to the display
    myViewer->addCoordinateSystem(scale, transformation, id, viewPort);
}

/***********************************************************************************************************************
 * @brief Add a line to the display
 * @param[in] x1 the x coordinate of the origin point
 * @param[in] y1 the y coordinate of the origin point
 * @param[in] z1 the z coordinate of the origin point
 * @param[in] x2 the x coordinate of the direction point
 * @param[in] y2 the y coordinate of the direction point
 * @param[in] z2 the z coordinate of the direction point
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered line (default: 1.0)
 * @param[in] lineWidth width of the rendered line (default: 1.0)
 * @param[in] id the unique identifier of the line (default: "line")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addLine(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b, double opacity, double lineWidth, const string &id, int viewPort)
{
    pcl::PointXYZ p1(x1, y1, z1);
    pcl::PointXYZ p2(x2, y2, z2);

    // add the line to the display
    myViewer->addLine(p1, p2, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, r, g, b, id);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, lineWidth, id);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id);
}

/***********************************************************************************************************************
 * @brief Add a polygon to the display
 *
 * Adds a polygon to the rendering window by connecting the vertices in order
 *
 * @param[in] vertices pointer to the cloud containing polygon vertices
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered line (default: 1.0)
 * @param[in] lineWidth width of the rendered line (default: 1.0)
 * @param[in] drawSolid renders the polygon as a solid (default: false)
 * @param[in] id the unique identifier of the line (default: "polygon")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addPolygon(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &vertices, double r, double g, double b, double opacity, double lineWidth, bool drawSolid, const string &id, int viewPort)
{
    myViewer->addPolygon<pcl::PointXYZRGBA>(vertices, r, g, b, id, viewPort);

    // render the polygon as a solid or wireframe
    if(drawSolid)
    {
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, lineWidth, id, viewPort);
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE, id, viewPort);
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, r, g, b, id, viewPort);
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id, viewPort);
    }
    else
    {
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, lineWidth, id, viewPort);
    }
}

/***********************************************************************************************************************
 * @brief Add a box to the rendering window
 *
 * Adds a box frame to the rendering window with configurable position, orientation, dimensions, and color
 *
 * @param[in] x the x coordinate of the box center
 * @param[in] x the y coordinate of the box center
 * @param[in] x the z coordinate of the box center
 * @param[in] roll the roll coordinate of the box
 * @param[in] pitch the pitch coordinate of the box
 * @param[in] yaw the yaw coordinate of the box
 * @param[in] width the width dimension of the box
 * @param[in] height the height dimension of the box
 * @param[in] depth the depth dimension of the box
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] frameSize the size of the box frame (default: 1.0)
 * @param[in] drawSolid renders the cube as a solid (default: false)
 * @param[in] id the unique identifier of the rendered box (default: "box")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addBox(double x, double y, double z, double roll, double pitch, double yaw, double width, double height, double depth, double r, double g, double b, double opacity, double frameSize, bool drawSolid, const string &id, int viewPort)
{
    // obtain a vector representation of the translation
    const Eigen::Vector3f translation(x, y, z);

    // obtain a quaternion representation of the rotation
    double qw, qx, qy, qz;
    Eigen::Quaternionf rotation;
    Eigen::AngleAxisf aaZ(static_cast<float>(yaw), Eigen::Vector3f::UnitZ());
    Eigen::AngleAxisf aaY(static_cast<float>(pitch), Eigen::Vector3f::UnitY());
    Eigen::AngleAxisf aaX(static_cast<float>(roll), Eigen::Vector3f::UnitX());
    rotation = aaZ * aaY * aaX;

    // add a cube to the display
    myViewer->addCube(translation, rotation, width, height, depth, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, r, g, b, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, frameSize, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id, viewPort);
    if(drawSolid)
    {
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE, id, viewPort);
    }
}

/***********************************************************************************************************************
 * @brief Add a cuboid to the rendering window
 *
 * Adds a cuboid to the rendering window with configurable position, orientation, dimensions, and color
 *
 * @param[in] position the translation of the frame relative to the origin
 * @param[in] orientation the quaternion orientation of the frame relative to the origin
 * @param[in] width the width dimension of the box
 * @param[in] height the height dimension of the box
 * @param[in] depth the depth dimension of the box
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] frameSize the size of the box frame (default: 1.0)
 * @param[in] drawSolid renders the cube as a solid (default: false)
 * @param[in] id the unique identifier of the rendered box (default: "box")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addBox(const Eigen::Vector3f &position, const Eigen::Quaternionf &orientation, double width, double height, double depth, double r, double g, double b, double opacity, double frameSize, bool drawSolid, const string &id, int viewPort)
{
    // add a cube to the display
    myViewer->addCube(position, orientation, width, height, depth, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, r, g, b, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, frameSize, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id, viewPort);
    if(drawSolid)
    {
        myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_SURFACE, id, viewPort);
    }
}

/***********************************************************************************************************************
 * @brief Add a sphere to the rendering window
 *
 * Adds a sphere to the rendering window with configurable position, radius, and color
 *
 * @param[in] x the x coordinate of the sphere center
 * @param[in] x the y coordinate of the sphere center
 * @param[in] x the z coordinate of the sphere center
 * @param[in] radius the radius of the sphere
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered sphere (default: 1.0)
 * @param[in] id the unique identifier of the rendered sphere (default: "sphere")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addSphere(double x, double y, double z, double radius, double r, double g, double b, double opacity, const string &id, int viewPort)
{
    // add a cube to the display
    CloudVisualizer::addSphere(Eigen::Vector3f(x, y, z), radius, r, g, b, opacity, id, viewPort);
}

/***********************************************************************************************************************
 * @brief Add a sphere to the rendering window
 *
 * Adds a sphere to the rendering window with configurable position, radius, and color
 *
 * @param[in] position the translation of the frame relative to the origin
 * @param[in] radius the radius of the sphere
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered sphere (default: 1.0)
 * @param[in] id the unique identifier of the rendered sphere (default: "sphere")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addSphere(const Eigen::Vector3f &position, double radius, double r, double g, double b, double opacity, const string &id, int viewPort)
{
    // add a sphere to the display
    pcl::PointXYZ point(position[0], position[1], position[2]);
    myViewer->addSphere(point, radius, r, g, b, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id);
}

/***********************************************************************************************************************
 * @brief Add a cuboid to the rendering window
 *
 * Adds a cuboid to the rendering window with configurable position, orientation, dimensions, and color
 *
 * @param[in] cornerFTL the front top left corner of the cuboid
 * @param[in] cornerFTR the front top right corner of the cuboid
 * @param[in] cornerFBL the front bottom left corner of the cuboid
 * @param[in] cornerFBR the front bottom right corner of the cuboid
 * @param[in] cornerBTL the back top left corner of the cuboid
 * @param[in] cornerBTR the back top right corner of the cuboid
 * @param[in] cornerBBL the back bottom left corner of the cuboid
 * @param[in] cornerBBR the back bottom right corner of the cuboid
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] frameSize the size of the box frame (default: 1.0)
 * @param[in] id the unique identifier of the rendered box (default: "box")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addCuboid(const Eigen::Vector4f &cornerFTL, const Eigen::Vector4f &cornerFTR, const Eigen::Vector4f &cornerFBL, const Eigen::Vector4f &cornerFBR, const Eigen::Vector4f &cornerBTL, const Eigen::Vector4f &cornerBTR, const Eigen::Vector4f &cornerBBL, const Eigen::Vector4f &cornerBBR, double r, double g, double b, double opacity, double frameSize, const string &id, int viewPort)
{
    std::stringstream ss;

    // draw the front face edges
    ss << id << "_front_top";
    CloudVisualizer::addLine(cornerFTL[0], cornerFTL[1], cornerFTL[2], cornerFTR[0], cornerFTR[1], cornerFTR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_front_left";
    CloudVisualizer::addLine(cornerFTL[0], cornerFTL[1], cornerFTL[2], cornerFBL[0], cornerFBL[1], cornerFBL[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_front_right";
    CloudVisualizer::addLine(cornerFTR[0], cornerFTR[1], cornerFTR[2], cornerFBR[0], cornerFBR[1], cornerFBR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_front_bottom";
    CloudVisualizer::addLine(cornerFBL[0], cornerFBL[1], cornerFBL[2], cornerFBR[0], cornerFBR[1], cornerFBR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);

    // draw the back face edges
    ss.str("");
    ss << id << "_back_top";
    CloudVisualizer::addLine(cornerBTL[0], cornerBTL[1], cornerBTL[2], cornerBTR[0], cornerBTR[1], cornerBTR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_back_left";
    CloudVisualizer::addLine(cornerBTL[0], cornerBTL[1], cornerBTL[2], cornerBBL[0], cornerBBL[1], cornerBBL[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_back_right";
    CloudVisualizer::addLine(cornerBTR[0], cornerBTR[1], cornerBTR[2], cornerBBR[0], cornerBBR[1], cornerBBR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_back_bottom";
    CloudVisualizer::addLine(cornerBBL[0], cornerBBL[1], cornerBBL[2], cornerBBR[0], cornerBBR[1], cornerBBR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);

    // draw the depth edges
    ss.str("");
    ss << id << "_top_left";
    CloudVisualizer::addLine(cornerFTL[0], cornerFTL[1], cornerFTL[2], cornerBTL[0], cornerBTL[1], cornerBTL[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_top_right";
    CloudVisualizer::addLine(cornerFTR[0], cornerFTR[1], cornerFTR[2], cornerBTR[0], cornerBTR[1], cornerBTR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_bottom_left";
    CloudVisualizer::addLine(cornerFBL[0], cornerFBL[1], cornerFBL[2], cornerBBL[0], cornerBBL[1], cornerBBL[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
    ss.str("");
    ss << id << "_bottom_right";
    CloudVisualizer::addLine(cornerFBR[0], cornerFBR[1], cornerFBR[2], cornerBBR[0], cornerBBR[1], cornerBBR[2], r, g, b, opacity, frameSize, ss.str().c_str(), viewPort);
}

/***********************************************************************************************************************
 * @brief Add a plane to the viewer
 *
 * Adds a plane specified by the input coefficients to the viewer window
 *
 * @param[in] plane the planar coefficients of the target plane in the form Ax+By+Cz+D=0
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] id the unique identifier of the rendered plane (default: "plane")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addPlane(const Eigen::Vector4f &plane, double r, double g, double b, double opacity, const string &id, int viewPort)
{
    pcl::ModelCoefficients coefficients;
    coefficients.values.resize(4);
    coefficients.values.at(0) = plane[0];
    coefficients.values.at(1) = plane[1];
    coefficients.values.at(2) = plane[2];
    coefficients.values.at(3) = plane[3];

     // add a plane to the display
    myViewer->addPlane(coefficients, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, r, g, b, id, viewPort);
    myViewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id);
}

/***********************************************************************************************************************
 * @brief Add an occupancy grid to the viewer
 *
 * Adds an occupancy grid represented by the input octree structure
 *
 * @param[in] octree the input octree structure
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] frameSize the size of the box frame (default: 1.0)
 * @param[in] id the unique identifier of the rendered box (default: "box")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addOccupancyGrid(const pcl::octree::OctreePointCloud<pcl::PointXYZRGBA> &octree, double r, double g, double b, double opacity, double frameSize, const string &id, int viewPort)
{
    double leafSize = octree.getResolution();
    pcl::octree::OctreePointCloud<pcl::PointXYZRGBA>::AlignedPointTVector vcs;
    octree.getOccupiedVoxelCenters(vcs);

    // render each leaf node as a cube or sphere
    for(int i = 0; i < vcs.size(); i++)
    {
        std::stringstream ss;
        ss << id << "_leaf_" << i;
        CloudVisualizer::addBox(vcs.at(i).x, vcs.at(i).y, vcs.at(i).z, 0, 0, 0, leafSize, leafSize, leafSize, r, g, b, opacity, frameSize, false, ss.str(), viewPort);
    }
}

/***********************************************************************************************************************
 * @brief Add an occupancy grid to the viewer
 *
 * Adds an occupancy grid represented by the input octree structure
 *
 * @param[in] octree pointer to the input octree structure
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] frameSize the size of the box frame (default: 1.0)
 * @param[in] id the unique identifier of the rendered box (default: "box")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addOccupancyGrid(const pcl::octree::OctreePointCloud<pcl::PointXYZRGBA>::ConstPtr octree, double r, double g, double b, double opacity, double frameSize, const string &id, int viewPort)
{
    double leafSize = octree->getResolution();
    pcl::octree::OctreePointCloud<pcl::PointXYZRGBA>::AlignedPointTVector vcs;
    octree->getOccupiedVoxelCenters(vcs);

    // render each leaf node as a cube
    for(int i = 0; i < vcs.size(); i++)
    {
        std::stringstream ss;
        ss << id << "_leaf_" << i;
        CloudVisualizer::addBox(vcs.at(i).x, vcs.at(i).y, vcs.at(i).z, 0, 0, 0, leafSize, leafSize, leafSize, r, g, b, opacity, frameSize, false, ss.str(), viewPort);
    }
}

/***********************************************************************************************************************
 * @brief Add an occupancy grid to the viewer, represented by centroid spheres
 *
 * Adds an occupancy grid represented by the input octree structure
 *
 * @param[in] octree the input octree structure
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] frameSize the size of the box frame (default: 1.0)
 * @param[in] id the unique identifier of the rendered box (default: "centroid")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addOccupancyGridSpheres(const pcl::octree::OctreePointCloud<pcl::PointXYZRGBA> &octree, double r, double g, double b, double opacity, const string &id, int viewPort)
{
    double leafSize = octree.getResolution();
    pcl::octree::OctreePointCloud<pcl::PointXYZRGBA>::AlignedPointTVector vcs;
    octree.getOccupiedVoxelCenters(vcs);

    // render each leaf node as a cube or sphere
    for(int i = 0; i < vcs.size(); i++)
    {
        std::stringstream ss;
        ss << id << "_leaf_" << i;
        CloudVisualizer::addSphere(vcs.at(i).x, vcs.at(i).y, vcs.at(i).z, leafSize*0.5, r, g, b, opacity, ss.str(), viewPort);
    }
}

/***********************************************************************************************************************
 * @brief Add a polygon mesh to the viewer
 * @param[in] mesh the polygon mesh to visualize
 * @param[in] r the red color component (default: 255.0)
 * @param[in] g the green color component (default: 255.0)
 * @param[in] b the blue color component (default: 255.0)
 * @param[in] opacity the opacity of the rendered box frame (default: 1.0)
 * @param[in] id the unique identifier of the rendered mesh (default: "mesh")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::addPolygonMesh(const pcl::PolygonMesh::ConstPtr &mesh, double r, double g, double b, double opacity, const string &id, int viewPort)
{
    myViewer->addPolygonMesh(*mesh, id, viewPort);
    myViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, opacity, id, viewPort);
    myViewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, r, g, b, id, viewPort);
}

/***********************************************************************************************************************
 * @brief Remove a polygon mesh from the viewer
 * @param[in] id the unique identifier of the rendered mesh (default: "mesh")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::removePolygonMesh(const string &id, int viewPort)
{
    //myViewer->removePolygonMesh(id, viewPort);
    myViewer->removeShape(id, viewPort);
}

/***********************************************************************************************************************
 * @brief Remove a cloud from the viewer
 *
 * Remove a point cloud on the screen from the given viewport
 *
 * @param[in] id the name of the point cloud to remove (default: "cloud")
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::removePointCloud(const string &id, int viewPort)
{
    myViewer->removePointCloud(id, viewPort);
}

/***********************************************************************************************************************
 * @brief Remove clouds from viewer
 *
 * Remove all point cloud data on screen from the given viewport.
 *
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::removeAllClouds(int viewPort)
{
    myViewer->removeAllPointClouds(viewPort);
}

/***********************************************************************************************************************
 * @brief Remove shapes from viewer
 *
 * Remove all 3D shape data on screen from the given viewport.
 *
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::removeAllShapes(int viewPort)
{
    myViewer->removeAllShapes(viewPort);
}

/***********************************************************************************************************************
 * @brief Remove a shape from viewer
 *
 * Remove a 3D shape on screen from the given viewport.
 *
 * @param[in] name the name of the point cloud to remove
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::removeShape(const string &id, int viewPort)
{
    myViewer->removeShape(id, viewPort);
}

/***********************************************************************************************************************
 * @brief Remove a coordinate system from viewer
 *
 * Remove a 3D coordinate system from the given viewport.
 *
 * @param[in] name the name of the point cloud to remove (default: frame)
 * @param[in] viewPort the viewPort id if using multiple viewports (default: 0)
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::removeCoordinateFrame(const string &id, int viewPort)
{
    myViewer->removeCoordinateSystem(id, viewPort);
}

/***********************************************************************************************************************
 * @brief gets the color components of an internally generated color with a given index
 * @param[in] index the index of the desired color
 * @param[out] r the red color compoment
 * @param[out] g the green color compoment
 * @param[out] b the blue color compoment
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
void CloudVisualizer::getColor(int index, int &r, int &g, int &b)
{
    // generate the color
    switch(index)
    {
        case 0:
            r = 255; g = 0; b = 0;
            break;
        case 1:
            r = 0; g = 255; b = 0;
            break;
        case 2:
            r = 0; g = 0; b = 255;
            break;
        case 3:
            r = 255; g = 255; b = 0;
            break;
        case 4:
            r = 0; g = 255; b = 255;
            break;
        case 5:
            r = 255; g = 0; b = 255;
            break;
        case 6:
            r = 255; g = 255; b = 255;
            break;
        default:
            std::srand(index);
            r = std::rand() % 255;
            g = std::rand() % 255;
            b = std::rand() % 255;
            break;
    }
}



