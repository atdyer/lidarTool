#ifndef DATATYPES_H
#define DATATYPES_H

// Point Cloud Library
#include <pcl/common/common.h>
#include <pcl/common/transforms.h>
#include <pcl/common/geometry.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>

// Eigen Library
#include <eigen3/Eigen/Eigen>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

enum {MaxRecentFiles = 5};

#endif // DATATYPES_H
