#include "RegularGrid.h"

RegularGrid::RegularGrid(QObject *parent):
	Grid(parent)
{
	rows = 10;
	cols = 10;
}


RegularGrid::~RegularGrid()
{
	for (int i=0; i<grid.length(); ++i)
		delete grid[i];
}


PointCloudT *RegularGrid::getCloudData(int row, int col)
{
	return grid[row*cols + col]->getCloud();
}


void RegularGrid::run()
{
	if (!cloud)
	{
		emit gridError("Regular Grid Error: No input cloud");
		return;
	}

	emit gridStarted();

	// Calculate load bar stuff
	int maxProgress = rows*cols + 2*cloud->points.size();
	int onePercent = 0.01*maxProgress;
	int progress = 0;
	int percent = 0;

	// Calculate the bounding box
//	PointT minPoint, maxPoint;
//	pcl::getMinMax3D(*cloud, minPoint, maxPoint);
	minX = 99999;
	minY = 99999;
	maxX = -99999;
	maxY = -99999;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		if (cloud->points[i].x < minX)
			minX = cloud->points[i].x;
		else if (cloud->points[i].x > maxX)
			maxX = cloud->points[i].x;
		if (cloud->points[i].y < minY)
			minY = cloud->points[i].y;
		else if (cloud->points[i].y > maxY)
			maxY = cloud->points[i].y;

		++progress;
		if (progress % onePercent == 0)
			emit gridProgress(percent++);
	}

//	minX = minPoint.x;
//	maxX = maxPoint.x;
//	minY = minPoint.y;
//	maxY = maxPoint.y;

	// Initialize the empty grid
	grid.reserve(rows*cols);
	for (int row=0; row<rows; ++row)
		for (int col=0; col<cols; ++col)
			grid.append(0);

	// Calculate grid cell size and spacing
	double cellWidth = (maxX - minX) / cols;
	double cellHeight = (maxY - minY) / rows;

	// Create the grid cells
	for (int row=0; row<rows; ++row)
	{
		for (int col=0; col<cols; ++col)
		{
			double cellX = minX + cellWidth*col;
			double cellY = minY + cellHeight*row;
			GridCell *cell = new GridCell(cellX, cellX + cellWidth,
						      cellY, cellY + cellHeight,
						      0);
			grid[row*cols + col] = cell;

			++progress;
			if (progress % onePercent == 0)
				emit gridProgress(percent++);
		}
	}

	// Add the points to the grid
	for (int i=0; i<cloud->points.size(); ++i)
	{
		PointT newPoint(cloud->points[i]);
		int gridRow = floor((newPoint.x - minX) / cellWidth);
		int gridCol = floor((newPoint.y - minY) / cellHeight);

		if (gridRow == rows) gridRow -= 1; // The maximum point is going to want
		if (gridCol == cols) gridCol -= 1; // to be in a cell outside of the grid

//		std::cout << gridRow << '\t' << gridCol << std::endl;

		grid[gridRow*rows + gridCol]->addPoint(newPoint);

		++progress;
		if (progress % onePercent == 0)
			emit gridProgress(percent++);
	}

	emit gridFinished(this);
}


void RegularGrid::setNumRows(int rows)
{
	this->rows = rows;
}


void RegularGrid::setNumCols(int cols)
{
	this->cols = cols;
}
