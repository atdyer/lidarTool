#include "Filter.h"

Filter::Filter(QObject *parent) :
	QObject(parent)
{
	cloud = 0;
}


void Filter::setInputCloud(PointCloudT *cloud)
{
	this->cloud = cloud;
}


void Filter::setInputGrid(Grid *grid)
{
	this->grid = grid;
}
