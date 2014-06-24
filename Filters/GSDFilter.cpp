#include "GSDFilter.h"

GSDFilter::GSDFilter(PointCloudT *pointCloud, QObject *parent) :
	QObject(parent),
	cloud(pointCloud),
	grid(0)
{
	filterSteps.append("Creating Grid");
	filterSteps.append("Calculating Bounding Box");
	filterSteps.append("Finishing Up");
}


void GSDFilter::run()
{
	emit started();

	// Create the grid and add data
	Initialize();

	// Do filtering here

	// Finish up
	emit progress(filterSteps.at(2), 100*(2.0/filterSteps.length()));
	if (grid)
		delete grid;

	emit finished();
}


void GSDFilter::SetGridDimensions(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;
}


void GSDFilter::Initialize()
{
	// Create the grid
	if (!grid)
	{
		emit progress(filterSteps.at(0), 100*(0.0/filterSteps.length()));
		grid = new Grid(rows, cols, 0);

		emit progress(filterSteps.at(1), 100*(1.0/filterSteps.length()));
		grid->AddData(cloud);
	}
}
