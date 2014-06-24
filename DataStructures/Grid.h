#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QVector>

#include <iostream>

#include "../DataTypes.h"

typedef QVector<PointCloudT> gridCol;

class Grid : public QObject
{
		Q_OBJECT
	public:
		explicit Grid(int rows, int cols, QObject *parent = 0);

		void	AddData(PointCloudT *cloud);

	private:

		int	numRows;
		int	numCols;

		QVector<gridCol>	grid;

	signals:

	public slots:

};

#endif // GRID_H
