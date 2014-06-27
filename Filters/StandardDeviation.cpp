#include "StandardDeviation.h"

StandardDeviation::StandardDeviation(QObject *parent) :
	Filter(parent)
{
	numStdDevs = 1.0;
}


void StandardDeviation::run()
{
	if (cloud)
		runCloud();
	else if (grid)
		runGrid();
}


void StandardDeviation::runCloud()
{
	// Check for a cloud to work on
	if (!cloud)
	{
		emit filterError("Standard Deviation Error: No input cloud");
		return;
	}

	// Calculate load bar stuff
	int maxProgress = cloud->points.size()*3;
	int onePercent = 0.01*maxProgress;
	int progress = 0;
	int percent = 0;

	// Start the filter
	emit filterStarted();

	// Loop through the points to calculate the average elevation
	double sum = 0.0;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		sum += cloud->points[i].z;
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Calculate the average
	double average = sum / (double)cloud->points.size();

	// Loop through the points to calculate the sum of distances from the average elevation squared
	sum = 0.0;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		double diff = average - cloud->points[i].z;
		sum += diff*diff;
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Calculate the standard deviation
	double stdDev = sqrt(sum/(double)cloud->points.size());

	// Loop through the points, creating a new point cloud
	PointCloudT *newCloud = new PointCloudT;
	newCloud->clear();
	double minVal = average - numStdDevs*stdDev;
	double maxVal = average + numStdDevs*stdDev;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		if (!(cloud->points[i].z > maxVal || cloud->points[i].z < minVal))
		{
			PointT newPoint (cloud->points[i]);
			newCloud->points.push_back(newPoint);
		}
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	emit filterFinished(newCloud);
}


void StandardDeviation::runGrid()
{
	// Fake it
	emit filterStarted();

	for (int i=0; i<1000; ++i)
		if (i%10 == 0)
			emit filterProgress(i/10);

	emit filterFinished(grid);
}


void StandardDeviation::setNumStandardDeviations(double stdDev)
{
	numStdDevs = stdDev;
}
