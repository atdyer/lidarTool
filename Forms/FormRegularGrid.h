#ifndef FORMREGULARGRID_H
#define FORMREGULARGRID_H

#include <QWidget>

namespace Ui {
	class FormRegularGrid;
}

class FormRegularGrid : public QWidget
{
		Q_OBJECT

	public:
		explicit FormRegularGrid(QWidget *parent = 0);
		~FormRegularGrid();

	private:
		Ui::FormRegularGrid *ui;

	signals:

		void	numRowsChanged(int);
		void	numColsChanged(int);
};

#endif // FORMREGULARGRID_H
