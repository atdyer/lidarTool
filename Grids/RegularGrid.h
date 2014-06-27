#ifndef REGULARGRID_H
#define REGULARGRID_H

#include "Grid.h"

class RegularGrid : public Grid
{
		Q_OBJECT
	public:
		explicit RegularGrid(QObject *parent = 0);
		~RegularGrid();
		static QString	GetGridName() {return "Regular Grid";}

		PointCloudT	*getCloudData(int row, int col);

		void	run();

	protected:

		int			rows;
		int			cols;
		QList<GridCell*>	grid;

	public slots:

		void	setNumRows(int rows);
		void	setNumCols(int cols);
};

#endif // REGULARGRID_H
