#ifndef STANDARDDEVIATION_H
#define STANDARDDEVIATION_H

#include "../Filters/Filter.h"

class StandardDeviation : public Filter
{
	public:
		StandardDeviation();
		static QString	GetFilterName() {return "Standard Deviation";}

		void	run();

};

#endif // STANDARDDEVIATION_H
