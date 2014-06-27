#include "DialogFilter.h"
#include "../../lidar-build/ui_DialogFilter.h"

DialogFilter::DialogFilter(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogFilter)
{
	ui->setupUi(this);

	Initialize();
}

DialogFilter::~DialogFilter()
{
	delete ui;
}


void DialogFilter::SetCloud(PointCloudT *newCloud)
{
	cloud = newCloud;
}


void DialogFilter::Initialize()
{
	// Initialize defaults
	cloud = 0;
	currentStep = 0;

	// Hide the progress bars
	ui->progressMain->setVisible(false);
	ui->progressSub->setVisible(false);

	// Add the filters and grids to the tree
	ui->itemTree->setColumnCount(1);
	filterParent = new QTreeWidgetItem(ui->itemTree);
	filterParent->setText(0, "Filters");
	gridParent = new QTreeWidgetItem(ui->itemTree);
	gridParent->setText(0, "Grids");

	QTreeWidgetItem *averageDeviation = new QTreeWidgetItem(filterParent);
	QTreeWidgetItem *standardDeviation = new QTreeWidgetItem(filterParent);
	QTreeWidgetItem *regularGrid = new QTreeWidgetItem(gridParent);

	averageDeviation->setText(0, AverageDeviation::GetFilterName());
	standardDeviation->setText(0, StandardDeviation::GetFilterName());
	regularGrid->setText(0, RegularGrid::GetGridName());

	// Create the toolbar
	toolbar = new QToolBar(this);
	actiongroup = new QActionGroup(this);
	ui->toolbarLayout->insertWidget(0, toolbar);
	connect(actiongroup, SIGNAL(triggered(QAction*)), this, SLOT(selectFilter(QAction*)));

	// Connect buttons
	connect(ui->itemTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemSelected(QTreeWidgetItem*,int)));
	connect(ui->moveRightButton, SIGNAL(clicked()), this, SLOT(moveFilterRight()));
	connect(ui->moveLeftButton, SIGNAL(clicked()), this, SLOT(moveFilterLeft()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeFilter()));
	connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startFilter()));
}


void DialogFilter::AddAverageDeviationFilter()
{
	// Add Button
	QAction *action = new QAction(AverageDeviation::GetFilterName(), this);
	action->setCheckable(true);
	actiongroup->addAction(action);
	toolbar->addAction(action);
	action->setChecked(true);
	actionList.push_back(action);

	// Add Filter
	AverageDeviation *filter = new AverageDeviation();

	connect(filter, SIGNAL(filterError(QString)), this, SLOT(filterError(QString)));
	connect(filter, SIGNAL(filterFinished(PointCloudT*)), this, SLOT(filterFinished(PointCloudT*)));
	connect(filter, SIGNAL(filterProgress(int)), this, SLOT(filterProgress(int)));
	connect(filter, SIGNAL(filterStarted()), this, SLOT(filterStarted()));

	filterList.append(filter);
	itemList.append(filter);

	// Add Form
	FormAverageDeviation *form = new FormAverageDeviation(this);
	ui->stackedWidget->addWidget(form);
	ui->stackedWidget->setCurrentWidget(form);

	connect(form, SIGNAL(tendencyMeasureChanged(QString)), filter, SLOT(setTendencyMeasure(QString)));
	connect(form, SIGNAL(scalingFactorChanged(double)), filter, SLOT(setScalingFactor(double)));
	connect(form, SIGNAL(roundingValueChanged(QString)), filter, SLOT(setRoundingValue(QString)));
}


void DialogFilter::AddStandardDeviationFilter()
{
	// Add Button
	QAction *action = new QAction(StandardDeviation::GetFilterName(), this);
	action->setCheckable(true);
	toolbar->addAction(action);
	actiongroup->addAction(action);
	action->setChecked(true);
	actionList.push_back(action);

	// Add Filter
	StandardDeviation *filter = new StandardDeviation();

	connect(filter, SIGNAL(filterError(QString)), this, SLOT(filterError(QString)));
	connect(filter, SIGNAL(filterFinished(PointCloudT*)), this, SLOT(filterFinished(PointCloudT*)));
	connect(filter, SIGNAL(filterFinished(Grid*)), this, SLOT(gridFinished(Grid*)));
	connect(filter, SIGNAL(filterProgress(int)), this, SLOT(filterProgress(int)));
	connect(filter, SIGNAL(filterStarted()), this, SLOT(filterStarted()));

	filterList.append(filter);
	itemList.append(filter);

	// Add Form
	FormStandardDeviation *form = new FormStandardDeviation(this);
	ui->stackedWidget->addWidget(form);
	ui->stackedWidget->setCurrentWidget(form);

	connect(form, SIGNAL(numStandardDeviationsChanged(double)), filter, SLOT(setNumStandardDeviations(double)));
}


void DialogFilter::AddRegularGrid()
{
	// Add Button
	QAction *action = new QAction(RegularGrid::GetGridName(), this);
	action->setCheckable(true);
	toolbar->addAction(action);
	actiongroup->addAction(action);
	action->setChecked(true);
	actionList.push_back(action);

	// Add Grid
	RegularGrid *grid = new RegularGrid();

	connect(grid, SIGNAL(gridError(QString)), this, SLOT(gridError(QString)));
	connect(grid, SIGNAL(gridFinished(Grid*)), this, SLOT(gridFinished(Grid*)));
	connect(grid, SIGNAL(gridProgress(int)), this, SLOT(gridProgress(int)));
	connect(grid, SIGNAL(gridStarted()), this, SLOT(gridStarted()));

	gridList.append(grid);
	itemList.append(grid);

	// Add Form
	FormRegularGrid *form = new FormRegularGrid(this);
	ui->stackedWidget->addWidget(form);
	ui->stackedWidget->setCurrentWidget(form);

	connect(form, SIGNAL(numRowsChanged(int)), grid, SLOT(setNumRows(int)));
	connect(form, SIGNAL(numColsChanged(int)), grid, SLOT(setNumCols(int)));

}


void DialogFilter::itemSelected(QTreeWidgetItem *item, int)
{
	QString text = item->text(0);

	if (QString::compare(text, AverageDeviation::GetFilterName()) == 0)
	{
		AddAverageDeviationFilter();
	}
	else if (QString::compare(text, StandardDeviation::GetFilterName()) == 0)
	{
		AddStandardDeviationFilter();
	}
	else if (QString::compare(text, RegularGrid::GetGridName()) == 0)
	{
		AddRegularGrid();
	}
}


void DialogFilter::moveFilterRight()
{
	QAction *currentAction = actiongroup->checkedAction();
	int index = actionList.indexOf(currentAction);
	QWidget *widget = ui->stackedWidget->widget(index);
	if (index < actionList.length() - 2)
	{
		QAction *rightAction = actionList.at(index+2);
		toolbar->removeAction(currentAction);
		toolbar->insertAction(rightAction, currentAction);
		actionList.move(index, index+1);
		filterList.move(index, index+1);
		ui->stackedWidget->removeWidget(widget);
		ui->stackedWidget->insertWidget(index+1, widget);
		ui->stackedWidget->setCurrentIndex(index+1);
	} else if (index != actionList.length() - 1){
		toolbar->removeAction(currentAction);
		toolbar->addAction(currentAction);
		actionList.move(index, index+1);
		filterList.move(index, index+1);
		ui->stackedWidget->removeWidget(widget);
		ui->stackedWidget->insertWidget(index+1, widget);
		ui->stackedWidget->setCurrentIndex(index+1);
	}
}


void DialogFilter::moveFilterLeft()
{
	QAction *currentAction = actiongroup->checkedAction();
	int index = actionList.indexOf(currentAction);
	QWidget *widget = ui->stackedWidget->widget(index);
	if (index > 0)
	{
		QAction *leftAction = actionList.at(index-1);
		toolbar->removeAction(currentAction);
		toolbar->insertAction(leftAction, currentAction);
		actionList.move(index, index-1);
		filterList.move(index, index-1);
		ui->stackedWidget->removeWidget(widget);
		ui->stackedWidget->insertWidget(index-1, widget);
		ui->stackedWidget->setCurrentIndex(index-1);
	}
}


void DialogFilter::removeFilter()
{
	QAction *currentAction = actiongroup->checkedAction();
	int index = actionList.indexOf(currentAction);
	Filter *currentFilter = filterList.takeAt(index);
	toolbar->removeAction(currentAction);
	actiongroup->removeAction(currentAction);
	actionList.removeOne(currentAction);
	ui->stackedWidget->removeWidget(ui->stackedWidget->widget(index));

	if (currentFilter)
		delete currentFilter;

	if (actionList.size() > 0)
	{
		if (index < actionList.size())
		{
			QAction *nextAction = actionList.at(index);
			nextAction->setChecked(true);
		} else {
			QAction *nextAction = actionList.last();
			nextAction->setChecked(true);
		}

	}
}


void DialogFilter::selectFilter(QAction *action)
{
	int index = actionList.indexOf(action);
	ui->stackedWidget->setCurrentIndex(index);
}


void DialogFilter::startFilter()
{
	if (!itemList.isEmpty())
	{
		// Prevent the user from screwing things up
		ui->itemTree->setEnabled(false);

		// Show the progress bars
		ui->progressMain->setVisible(true);
		ui->progressSub->setVisible(true);
		ui->progressMain->setFormat("");
		ui->progressMain->setMaximum(actionList.length()*100);
		ui->progressMain->setValue(0);
		ui->progressSub->setValue(0);

		// Setup the first step
		QObject *firstStep = itemList.first();
		Filter *firstFilter = 0;
		for (int i=0; i<filterList.length(); ++i)
			if (firstStep == filterList[i])
				firstFilter = filterList[i];
		Grid *firstGrid = 0;
		for (int i=0; i<gridList.length(); ++i)
			if (firstStep == gridList[i])
				firstGrid = gridList[i];

		if (firstFilter)
		{
			firstFilter->setAutoDelete(false);
			firstFilter->setInputCloud(cloud);
		}

		else if (firstGrid)
		{
			firstGrid->setAutoDelete(false);
			firstGrid->setInputCloud(cloud);
		}

		else
			return;

		// Setup the last step
		QObject *lastStep = itemList.last();
		Filter *lastFilter = 0;
		for (int i=0; i<filterList.size(); ++i)
			if (lastStep == filterList[i])
				lastFilter = filterList[i];
		Grid *lastGrid = 0;
		for (int i=0; i<gridList.size(); ++i)
			if (lastStep == gridList[i])
				lastGrid = gridList[i];

		if (lastFilter)
			connect(lastFilter, SIGNAL(filterFinished(PointCloudT*)), this, SIGNAL(filterComplete(PointCloudT*)));
		else if (lastGrid)
			connect(lastGrid, SIGNAL(gridFinished(Grid*)), this, SIGNAL(filterComplete(Grid*)));
		else
			return;

		if (firstFilter)
			QThreadPool::globalInstance()->start(firstFilter);
		else if (firstGrid)
			QThreadPool::globalInstance()->start(firstGrid);

	}

//	if (!filterList.isEmpty())
//	{
//		// Prevent the user from screwing things up
//		ui->itemTree->setEnabled(false);

//		// Show the progress bars
//		ui->progressMain->setVisible(true);
//		ui->progressSub->setVisible(true);
//		ui->progressMain->setFormat("");
//		ui->progressMain->setMaximum(actionList.length()*100);
//		ui->progressMain->setValue(0);
//		ui->progressSub->setValue(0);

//		// Setup the first filter
//		currentFilter = 0;
//		Filter *filter = filterList.first();
//		filter->setAutoDelete(false);

//		// Set the cloud in the first filter
//		filter->setInputCloud(cloud);

//		// Setup the last filter
//		Filter *lastFilter = filterList.last();
//		connect(lastFilter, SIGNAL(filterFinished(PointCloudT*)), this, SIGNAL(filterComplete(PointCloudT*)));

//		// Start the first filter
//		QThreadPool::globalInstance()->start(filter);
//	}

}


void DialogFilter::filterError(QString error)
{
	// Hide the progress bars and enable the button
	ui->progressMain->setVisible(false);
	ui->progressSub->setVisible(false);
	ui->itemTree->setEnabled(true);

	// Figure out which filter screwed up
	QObject *sentFrom = QObject::sender();
	for (int i=0; i<filterList.length(); ++i)
	{
		if (sentFrom == filterList[i])
		{
			QMessageBox dlg;
			dlg.setText("Error in step " + QString::number(i+1));
			dlg.setInformativeText(error);
			dlg.setStandardButtons(QMessageBox::Ok);
			dlg.setIcon(QMessageBox::Critical);
			dlg.exec();
		}
	}
}


void DialogFilter::filterStarted()
{
	ui->progressMain->setFormat("Filter Step " + QString::number(currentStep+1));
}


void DialogFilter::filterProgress(int val)
{
	ui->progressMain->setValue(100*currentStep + val);
	ui->progressSub->setValue(val);
}


void DialogFilter::filterFinished(PointCloudT *filterCloud)
{
	// Make the next step the current step
	++currentStep;

	// Make sure there is a next step
	if (currentStep < itemList.size())
	{
		QObject *nextStep = itemList.at(currentStep);
		Filter *filter = 0;
		Grid *grid = 0;
		for (int i=0; i<filterList.size(); ++i)
			if (nextStep == filterList[i])
				filter = filterList[i];
		for (int i=0; i<gridList.size(); ++i)
			if (nextStep == gridList[i])
				grid = gridList[i];

		if (filter)
		{
			// Send the point cloud to the next filter
			filter->setInputCloud(filterCloud);
			QThreadPool::globalInstance()->start(filter);
		}
		else if (grid)
		{
			// Send the point cloud to the next grid
			grid->setInputCloud(filterCloud);
			QThreadPool::globalInstance()->start(grid);
		}
	} else {

		QObject *lastStep = itemList.last();
		Filter *lastFilter = 0;
		for (int i=0; i<filterList.size(); ++i)
			if (lastStep == filterList[i])
				lastFilter = filterList[i];
		Grid *lastGrid = 0;
		for (int i=0; i<gridList.size(); ++i)
			if (lastStep == gridList[i])
				lastGrid = gridList[i];

		if (lastFilter)
			disconnect(lastFilter, SIGNAL(filterFinished(PointCloudT*)), this, SIGNAL(filterComplete(PointCloudT*)));
		else if (lastGrid)
			disconnect(lastGrid, SIGNAL(gridFinished(Grid*)), this, SIGNAL(filterComplete(Grid*)));

		this->cloud = filterCloud;
		ui->progressMain->setVisible(false);
		ui->progressSub->setVisible(false);
		ui->itemTree->setEnabled(true);

	}

//	// Make the next filter the current filter
//	++currentStep;

//	// Make sure there is a next filter
//	if (currentStep < filterList.size())
//	{
//		Filter *filter = filterList.at(currentStep);
//		filter->setAutoDelete(false);
//		if (filter)
//		{
//			// Send the point cloud to the next filter
//			filter->setInputCloud(cloud);
//			QThreadPool::globalInstance()->start(filter);
//		}
//	} else {

//		disconnect(filterList.last(), SIGNAL(filterFinished(PointCloudT*)), this, SIGNAL(filterComplete(PointCloudT*)));
//		this->cloud = cloud;
//		ui->progressMain->setVisible(false);
//		ui->progressSub->setVisible(false);
//		ui->itemTree->setEnabled(true);
//	}
}


void DialogFilter::gridError(QString error)
{

}


void DialogFilter::gridStarted()
{
	ui->progressMain->setFormat("Grid Step " + QString::number(currentStep+1));
}


void DialogFilter::gridProgress(int val)
{
	ui->progressMain->setValue(100*currentStep + val);
	ui->progressSub->setValue(val);
}


void DialogFilter::gridFinished(Grid *grid)
{
	// Make the next step the current step
	++currentStep;

	// Make sure there is a next step
	if (currentStep < itemList.size())
	{
		QObject *nextStep = itemList.at(currentStep);
		Filter *filter = 0;
		Grid *nextGrid = 0;
		for (int i=0; i<filterList.size(); ++i)
			if (nextStep == filterList[i])
				filter = filterList[i];
		for (int i=0; i<gridList.size(); ++i)
			if (nextStep == gridList[i])
				nextGrid = gridList[i];

		if (filter)
		{
			// Send the grid to the next filter
			filter->setInputGrid(grid);
			QThreadPool::globalInstance()->start(filter);
		}
		else if (nextGrid)
		{
			// Send the grid to the next grid
			nextGrid->setInputGrid(grid);
			QThreadPool::globalInstance()->start(nextGrid);
		}
	}
}
