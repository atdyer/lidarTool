#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QRunnable>

#include "../DataTypes.h"

class Filter : public QObject, public QRunnable
{
		Q_OBJECT
	public:
		explicit Filter(QObject *parent = 0);

	protected:

		PointCloudT	*cloud;

	public slots:

		void	setInputCloud(PointCloudT *cloud);

	signals:

		void	filterError(QString);
		void	filterStarted();
		void	filterProgress(int);
		void	filterFinished(PointCloudT*);
};

#endif // FILTER_H
