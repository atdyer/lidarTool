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
#include <QTreeWidget>
#include <QTreeWidgetItem>

// Grids
#include "../Grids/Grid.h"
#include "../Grids/RegularGrid.h"

// Filters
#include "../Filters/Filter.h"
#include "../Filters/AverageDeviation.h"
#include "../Filters/StandardDeviation.h"

// Forms
#include "../Forms/FormAverageDeviation.h"
#include "../Forms/FormStandardDeviation.h"
#include "../Forms/FormRegularGrid.h"

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
		QList<Grid*>	gridList;
		QList<QObject*>	itemList;
		QList<QAction*>	actionList;
		QList<QWidget*>	formList;

		QTreeWidgetItem *filterParent;
		QTreeWidgetItem *gridParent;

		int	currentStep;

		void	Initialize();
		void	AddAverageDeviationFilter();
		void	AddStandardDeviationFilter();
		void	AddRegularGrid();

	protected slots:

		void	itemSelected(QTreeWidgetItem* item, int);
		void	moveFilterRight();
		void	moveFilterLeft();
		void	removeFilter();
		void	selectFilter(QAction *action);
		void	startFilter();
		void	filterError(QString error);
		void	filterStarted();
		void	filterProgress(int val);
		void	filterFinished(PointCloudT *filterCloud);
		void	gridError(QString error);
		void	gridStarted();
		void	gridProgress(int val);
		void	gridFinished(Grid *grid);

	signals:

		void	filterComplete(PointCloudT*);
		void	filterComplete(Grid*);
};

#endif // DIALOGFILTER_H
