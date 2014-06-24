#ifndef GSDFILTER_H
#define GSDFILTER_H

#include <QObject>
#include <QRunnable>
#include <QDialog>

#include <iostream>

#include "../DataTypes.h"
#include "../DataStructures/Grid.h"

class GSDFilter : public QObject, public QRunnable
{
		Q_OBJECT
	public:
		explicit GSDFilter(PointCloudT *pointCloud, QObject *parent = 0);

		void	run();
		void	SetGridDimensions(int rows, int cols);

	private:

		PointCloudT	*cloud;
		Grid		*grid;

		QStringList	filterSteps;

		int	rows;
		int	cols;

		void	Initialize();

	signals:

		void	finished();
		void	progress(QString, int);
		void	started();

	public slots:

};

#endif // GSDFILTER_H
