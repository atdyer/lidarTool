#include "Grid.h"

Grid::Grid(int rows, int cols, QObject *parent) :
	QObject(parent),
	numRows(rows),
	numCols(cols),
	grid(rows, gridCol(cols))
{
	std::cout << "Grid created (" << grid.size() << ", " << grid.at(0).size() << ")" << std::endl;
}


void Grid::AddData(PointCloudT *cloud)
{
	if (cloud)
	{
		// Determine the bounding box in the x-y plane
		PointT minPoint, maxPoint;
		pcl::getMinMax3D(*cloud, minPoint, maxPoint);
		std::cout << "Min Point: " << minPoint << std::endl;
		std::cout << "Max Point: " << maxPoint << std::endl;
	}
}
