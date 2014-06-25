#ifndef AVERAGEDEVIATION_H
#define AVERAGEDEVIATION_H

#include "../Filters/Filter.h"

class AverageDeviation : public Filter
{
	public:
		AverageDeviation();
		static QString	GetFilterName() {return "Average Deviation";}

		void	run();
};

#endif // AVERAGEDEVIATION_H
