#ifndef DIALOGFILTER_H
#define DIALOGFILTER_H

// Qt
#include <QDialog>
#include <QList>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QThreadPool>
#include <QMessageBox>
#include <QProgressBar>
#include <QStackedWidget>

// Filters
#include "../Filters/Filter.h"
#include "../Filters/AverageDeviation.h"
#include "../Filters/StandardDeviation.h"

// Forms
#include "../Forms/FormAverageDeviation.h"
#include "../Forms/FormStandardDeviation.h"

// Point Cloud
#include "../DataTypes.h"

namespace Ui {
	class DialogFilter;
}

class DialogFilter : public QDialog
{
		Q_OBJECT

	public:
		explicit DialogFilter(QWidget *parent = 0);
		~DialogFilter();

		void	SetCloud(PointCloudT *newCloud);

	private:
		Ui::DialogFilter	*ui;
		QToolBar		*toolbar;
		QActionGroup		*actiongroup;

		PointCloudT	*cloud;

		QList<Filter*>	filterList;
		QList<QAction*>	actionList;
		QList<QWidget*>	formList;

		int	currentFilter;

		void	Initialize();
		void	AddAverageDeviationFilter();
		void	AddStandardDeviationFilter();

	protected slots:

		void	addFilter();
		void	moveFilterRight();
		void	moveFilterLeft();
		void	removeFilter();
		void	selectFilter(QAction *action);
		void	startFilter();
		void	filterError(QString error);
		void	filterStarted();
		void	filterProgress(int val);
		void	filterFinished(PointCloudT *cloud);

	signals:

		void	filterComplete(PointCloudT*);
};

#endif // DIALOGFILTER_H
