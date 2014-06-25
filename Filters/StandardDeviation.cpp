#include "StandardDeviation.h"

StandardDeviation::StandardDeviation()
{
}


void StandardDeviation::run()
{
	// Check for a cloud to work on
//	if (!cloud)
//	{
//		emit filterError("Standard Deviation Error: No input cloud");
//		return;
//	}

	// Start the filter
	emit filterStarted();

	// TESTING: Pretend to do stuff
	for (int i=0; i<100; ++i)
	{
		usleep(10000);
		emit filterProgress(i);
	}

	emit filterFinished(cloud);
}
