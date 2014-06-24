#ifndef DIALOGGSDFILTER_H
#define DIALOGGSDFILTER_H

#include <QDialog>
#include <QThreadPool>

#include "../DataTypes.h"
#include "../Filters/GSDFilter.h"

namespace Ui {
	class DialogGSDFilter;
}

class DialogGSDFilter : public QDialog
{
		Q_OBJECT

	public:
		explicit DialogGSDFilter(PointCloudT *pointCloud, QWidget *parent = 0);
		~DialogGSDFilter();

	private:
		Ui::DialogGSDFilter *ui;

		PointCloudT	*cloud;

	protected slots:

		void	applyFilter();
		void	hideProgress();
		void	progress(QString message, int value);
		void	showProgress();
};

#endif // DIALOGGSDFILTER_H
