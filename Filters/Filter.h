#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QRunnable>

#include "../DataTypes.h"
#include "../Grids/Grid.h"

class Filter : public QObject, public QRunnable
{
		Q_OBJECT
	public:
		explicit Filter(QObject *parent = 0);

	protected:

		PointCloudT	*cloud;
		Grid		*grid;

	public slots:

		void	setInputCloud(PointCloudT *cloud);
		void	setInputGrid(Grid *grid);

	signals:

		void	filterError(QString);
		void	filterStarted();
		void	filterProgress(int);
		void	filterFinished(PointCloudT*);
		void	filterFinished(Grid*);
};

#endif // FILTER_H
