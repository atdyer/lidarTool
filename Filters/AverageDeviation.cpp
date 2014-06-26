#include "AverageDeviation.h"

AverageDeviation::AverageDeviation()
{
	tendencyMeasure = 0;
	scalingFactor = 1.0;
	roundingValue = 0.1;
}


void AverageDeviation::run()
{
	// Check for a cloud to work on
	if (!cloud)
	{
		emit filterError("Average Deviation Error: No input cloud");
		return;
	}

	// Start the filter
	emit filterStarted();

	if (tendencyMeasure == 0)
	{
		PointCloudT *newCloud = runMean();
		if (newCloud)
			emit filterFinished(newCloud);
	}
	else if (tendencyMeasure == 1)
	{
		PointCloudT *newCloud = runMedian();
		if (newCloud)
			emit filterFinished(newCloud);
	}
	else if (tendencyMeasure == 2)
	{
		PointCloudT *newCloud = runMode();
		if (newCloud)
			emit filterFinished(newCloud);
	}
}


PointCloudT *AverageDeviation::runMean()
{

	// Calculate load bar stuff
	int maxProgress = cloud->points.size()*3;
	int onePercent = 0.01*maxProgress;
	int progress = 0;
	int percent = 0;

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

	// Loop through the points to calculate the average deviation
	sum = 0.0;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		sum += fabs(average - cloud->points[i].z);
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Calculate the average deviation
	double avgDev = sum / (double)cloud->points.size();

	// Loop through the points, creating a new point cloud
	PointCloudT *newCloud = new PointCloudT;
	newCloud->clear();
	double minVal = average - scalingFactor*avgDev;
	double maxVal = average + scalingFactor*avgDev;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		if (!(cloud->points[i].z > maxVal || cloud->points[i].z < minVal))
		{
			PointT newPoint;
			newPoint.x = cloud->points[i].x;
			newPoint.y = cloud->points[i].y;
			newPoint.z = cloud->points[i].z;
			newPoint.r = cloud->points[i].r;
			newPoint.g = cloud->points[i].g;
			newPoint.b = cloud->points[i].b;
			newPoint.a = cloud->points[i].a;
			newCloud->points.push_back(newPoint);
		}
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	return newCloud;
}


PointCloudT *AverageDeviation::runMedian()
{
	// Calculate load bar stuff
	int maxProgress = cloud->points.size()*3;
	int onePercent = 0.01*maxProgress;
	int progress = 0;
	int percent = 0;

	// Loop through the points to calculate the median elevation
	double sum = 0.0;
	std::vector<double> elevations;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		elevations.push_back(cloud->points[i].z);
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Calculate the median
	std::nth_element(elevations.begin(), elevations.begin() + elevations.size()/2, elevations.end());
	double median = elevations[elevations.size()/2];

	// Loop through the points to calculate the average deviation
	sum = 0.0;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		sum += fabs(median - cloud->points[i].z);
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Calculate the average deviation
	double avgDev = sum / (double)cloud->points.size();

	// Loop through the points, creating a new point cloud
	PointCloudT *newCloud = new PointCloudT;
	newCloud->clear();
	double minVal = median - scalingFactor*avgDev;
	double maxVal = median + scalingFactor*avgDev;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		if (!(cloud->points[i].z > maxVal || cloud->points[i].z < minVal))
		{
			PointT newPoint;
			newPoint.x = cloud->points[i].x;
			newPoint.y = cloud->points[i].y;
			newPoint.z = cloud->points[i].z;
			newPoint.r = cloud->points[i].r;
			newPoint.g = cloud->points[i].g;
			newPoint.b = cloud->points[i].b;
			newPoint.a = cloud->points[i].a;
			newCloud->points.push_back(newPoint);
		}
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	return newCloud;
}


PointCloudT *AverageDeviation::runMode()
{
	// Calculate load bar stuff
	int maxProgress = cloud->points.size()*4;
	int onePercent = 0.01*maxProgress;
	int progress = 0;
	int percent = 0;

	// Loop through the points to calculate the count the occurance of each value
	QMap<double, int> freqMap;
	for (int i=0; i<cloud->points.size(); ++i)
	{

		double val = roundValue(cloud->points[i].z);
		if (!freqMap.contains(val))
			freqMap[val] = 0;
		++freqMap[val];

		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Loop through the frequency map to find the mode
	double mode = -99999.0;
	int modeCount = 1;
	QMapIterator<double, int> it(freqMap);
	while (it.hasNext())
	{
		it.next();
		if (it.value() > modeCount)
		{
			mode = it.key();
			modeCount = it.value();
		}

		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Loop through the points to calculate the average deviation
	double sum = 0.0;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		sum += fabs(mode - cloud->points[i].z);
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	// Calculate the average deviation
	double avgDev = sum / (double)cloud->points.size();

	// Loop through the points, creating a new point cloud
	PointCloudT *newCloud = new PointCloudT;
	newCloud->clear();
	double minVal = mode - scalingFactor*avgDev;
	double maxVal = mode + scalingFactor*avgDev;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		if (!(cloud->points[i].z > maxVal || cloud->points[i].z < minVal))
		{
			PointT newPoint;
			newPoint.x = cloud->points[i].x;
			newPoint.y = cloud->points[i].y;
			newPoint.z = cloud->points[i].z;
			newPoint.r = cloud->points[i].r;
			newPoint.g = cloud->points[i].g;
			newPoint.b = cloud->points[i].b;
			newPoint.a = cloud->points[i].a;
			newCloud->points.push_back(newPoint);
		}
		++progress;
		if (progress % onePercent == 0)
			emit filterProgress(percent++);
	}

	return newCloud;
}


double AverageDeviation::roundValue(double value)
{
	return floor(value / roundingValue + 0.5) * roundingValue;
}


void AverageDeviation::setTendencyMeasure(QString measure)
{
	if (QString::compare(measure, "Mean", Qt::CaseInsensitive) == 0)
	{
		tendencyMeasure = 0;
	}
	else if (QString::compare(measure, "Median", Qt::CaseInsensitive) == 0)
	{
		tendencyMeasure = 1;
	}
	else if (QString::compare(measure, "Mode", Qt::CaseInsensitive) == 0)
	{
		tendencyMeasure = 2;
	}
}


void AverageDeviation::setScalingFactor(double factor)
{
	scalingFactor = factor;
}


void AverageDeviation::setRoundingValue(QString value)
{
	roundingValue = value.toDouble();
}
