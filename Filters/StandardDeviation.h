#ifndef STANDARDDEVIATION_H
#define STANDARDDEVIATION_H

#include "../Filters/Filter.h"

class StandardDeviation : public Filter
{
		Q_OBJECT
	public:
		StandardDeviation(QObject *parent = 0);
		static QString	GetFilterName() {return "Standard Deviation";}

		void	run();

	private:

		double	numStdDevs;

		void	runCloud();
		void	runGrid();

	public slots:

		void	setNumStandardDeviations(double stdDev);

};

#endif // STANDARDDEVIATION_H
