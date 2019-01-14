/*******************************************************************************************************************//**
 * @file CloudVisualizer.h
 * @brief Header file for the CloudVisualizer wrapper class
 *
 * This class provides a wrapper for PCL visualization functions
 *
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/

#ifndef CLOUDVISUALIZER_H
#define CLOUDVISUALIZER_H

#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/octree/octree.h>
#include <Eigen/Core>

using namespace std;

/*******************************************************************************************************************//**
 * @class CloudVisualizer
 *
 * @brief Class containing conventient wrapper functions for performing common tasks with PCL point clouds
 *
 * This class wraps several functions from pcl:visualization:PCLVisualizer for rendering point clouds and other 3D
 * annotations, such as shapes and text.
 *
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
class CloudVisualizer
{
private:

    boost::shared_ptr<pcl::visualization::PCLVisualizer> myViewer;

public:

    // constructors
    CloudVisualizer(const string &windowName="");

    // display mechanics
    void spin(int maxTimeMs=100);
    bool isRunning();
    void registerPointPickingCallback(void (*callback) (const pcl::visualization::PointPickingEvent&, void*), pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloud);
    void registerKeyboardCallback(void (*callback) (const pcl::visualization::KeyboardEvent&, void*));

    // rendering functions
    void addCloud(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloudIn, double pointSize=1.0, const string &id="cloud", int viewPort=0);
    void updateCloud(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud, const string &id="cloud");
    void addCoordinateFrame(const Eigen::Vector4f &position, const Eigen::Quaternionf &orientation, double scale=1.0, const string &id="frame", int viewPort=0);
    void addCoordinateFrame(double x, double y, double z, double roll, double pitch, double yaw, double scale=1.0, const string &id="frame", int viewPort=0);
    void addLine(double x1, double y1, double z1, double x2, double y2, double z2, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double lineWidth=1.0, const string &id="line", int viewPort=0);
    void addPolygon(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &vertices, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double lineWidth=1.0, bool drawSolid=false, const string &id="polygon", int viewPort=0);
    void addBox(double x, double y, double z, double roll, double pitch, double yaw, double width, double height, double depth, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double frameSize=1.0,  bool drawSolid=false, const string &id="box", int viewPort=0);
    void addBox(const Eigen::Vector3f &position, const Eigen::Quaternionf &orientation, double width, double height, double depth, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double frameSize=1.0, bool drawSolid=false, const string &id="box", int viewPort=0);
    void addSphere(double x, double y, double z, double radius, double r, double g, double b, double opacity, const string &id, int viewPort);
    void addSphere(const Eigen::Vector3f &position, double radius, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, const string &id="sphere", int viewPort=0);
    void addCuboid(const Eigen::Vector4f &cornerFTL, const Eigen::Vector4f &cornerFTR, const Eigen::Vector4f &cornerFBL, const Eigen::Vector4f &cornerFBR, const Eigen::Vector4f &cornerBTL, const Eigen::Vector4f &cornerBTR, const Eigen::Vector4f &cornerBBL, const Eigen::Vector4f &cornerBBR, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double frameSize=1.0, const string &id="cuboid", int viewPort=0);
    void addPlane(const Eigen::Vector4f &plane, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, const string &id="plane", int viewPort=0);
    void addOccupancyGrid(const pcl::octree::OctreePointCloud<pcl::PointXYZRGBA> &octree, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double frameSize=1.0, const string &id="octree", int viewPort=0);
    void addOccupancyGrid(const pcl::octree::OctreePointCloud<pcl::PointXYZRGBA>::ConstPtr octree, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, double frameSize=1.0, const string &id="octree", int viewPort=0);
    void addOccupancyGridSpheres(const pcl::octree::OctreePointCloud<pcl::PointXYZRGBA> &octree, double r, double g, double b, double opacity, const string &id, int viewPort);
    void addPolygonMesh(const pcl::PolygonMesh::ConstPtr &mesh, double r=255.0, double g=255.0, double b=255.0, double opacity=1.0, const string &id="mesh", int viewPort=0);
    void removePolygonMesh(const string &id="mesh", int viewPort=0);
    void removePointCloud(const string &id="cloud", int viewPort=0);
    void removeAllClouds(int viewPort=0);
    void removeAllShapes(int viewPort=0);
    void removeShape(const string &id, int viewPort=0);
    void removeCoordinateFrame(const string &id="frame", int viewPort=0);

    // misc
    static void getColor(int index, int &r, int &g, int &b);
};

#endif // CLOUDVISUALIZER_H
