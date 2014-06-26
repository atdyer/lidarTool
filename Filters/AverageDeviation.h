#ifndef AVERAGEDEVIATION_H
#define AVERAGEDEVIATION_H

#include <math.h>
#include <algorithm>

#include <QMap>

#include "../Filters/Filter.h"

class AverageDeviation : public Filter
{
		Q_OBJECT
	public:
		AverageDeviation();
		static QString	GetFilterName() {return "Average Deviation";}

		void	run();

	private:

		int	tendencyMeasure;
		double	scalingFactor;
		double	roundingValue;

		PointCloudT*	runMean();
		PointCloudT*	runMedian();
		PointCloudT*	runMode();

		double	roundValue(double value);

	public slots:

		void	setTendencyMeasure(QString measure);
		void	setScalingFactor(double factor);
		void	setRoundingValue(QString value);
};

#endif // AVERAGEDEVIATION_H
