#ifndef GRID_H
#define GRID_H

// Qt
#include <QObject>
#include <QRunnable>
#include <QList>

// PCL
#include "../DataTypes.h"

// Grid data
#include "GridCell.h"

class Grid : public QObject, public QRunnable
{
		Q_OBJECT
	public:
		explicit Grid(QObject *parent = 0);

	protected:

		double			minX;
		double			maxX;
		double			minY;
		double			maxY;
		PointCloudT		*cloud;
		Grid			*grid;

	public slots:

		void	setInputCloud(PointCloudT *cloud);
		void	setInputGrid(Grid *grid);

	signals:

		void	gridError(QString);
		void	gridStarted();
		void	gridProgress(int);
		void	gridFinished(Grid*);

};

#endif // GRID_H
