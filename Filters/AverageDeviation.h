#ifndef AVERAGEDEVIATION_H
#define AVERAGEDEVIATION_H

#include <math.h>

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

		PointCloudT*	runMean();
		PointCloudT*	runMedian();
		PointCloudT*	runMode();

	public slots:

		void	setTendencyMeasure(QString measure);
		void	setScalingFactor(double factor);
};

#endif // AVERAGEDEVIATION_H
