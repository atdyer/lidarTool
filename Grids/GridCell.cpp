#include "GridCell.h"

GridCell::GridCell(double xMin, double xMax, double yMin, double yMax, QObject *parent) :
	QObject(parent),
	xMin(xMin),
	xMax(xMax),
	yMin(yMin),
	yMax(yMax)
{
	cloud = new PointCloudT;
	cloudPtr.reset(cloud);
}


void GridCell::addPoint(PointT point)
{
	cloud->points.push_back(point);
}


PointCloudT* GridCell::getCloud()
{
	return cloud;
}
