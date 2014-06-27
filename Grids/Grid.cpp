#include "Grid.h"

Grid::Grid(QObject *parent) :
	QObject(parent),
	minX(0.0),
	maxX(1.0),
	minY(0.0),
	maxY(1.0),
	cloud(0)
{
}


void Grid::setInputCloud(PointCloudT *cloud)
{
	this->cloud = cloud;
}


void Grid::setInputGrid(Grid *grid)
{
	this->grid = grid;
}































