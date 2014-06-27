#ifndef GRIDCELL_H
#define GRIDCELL_H

// Qt
#include <QObject>

// PCL
#include "../DataTypes.h"

class GridCell : public QObject
{
		Q_OBJECT
	public:
		explicit GridCell(double xMin,
				  double xMax,
				  double yMin,
				  double yMax,
				  QObject *parent = 0);

		void		addPoint(PointT point);
		PointCloudT	*getCloud();

	protected:

		double	xMin;
		double	xMax;
		double	yMin;
		double	yMax;

		PointCloudT		*cloud;
		PointCloudT::Ptr	cloudPtr;

};

#endif // GRIDCELL_H
