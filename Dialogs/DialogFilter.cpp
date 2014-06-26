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
	currentFilter = 0;

	// Hide the progress bars
	ui->progressMain->setVisible(false);
	ui->progressSub->setVisible(false);

	// Add the filter names to the filter list
	ui->filterList->clear();
	ui->filterList->addItem(AverageDeviation::GetFilterName());
	ui->filterList->addItem(StandardDeviation::GetFilterName());

	// Create the toolbar
	toolbar = new QToolBar(this);
	actiongroup = new QActionGroup(this);
	ui->toolbarLayout->insertWidget(0, toolbar);
	connect(actiongroup, SIGNAL(triggered(QAction*)), this, SLOT(selectFilter(QAction*)));

	// Connect buttons
	connect(ui->addFilterButton, SIGNAL(clicked()), this, SLOT(addFilter()));
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
	connect(filter, SIGNAL(filterProgress(int)), this, SLOT(filterProgress(int)));
	connect(filter, SIGNAL(filterStarted()), this, SLOT(filterStarted()));

	filterList.append(filter);

	// Add Form
	FormStandardDeviation *form = new FormStandardDeviation(this);
	ui->stackedWidget->addWidget(form);
	ui->stackedWidget->setCurrentWidget(form);

	connect(form, SIGNAL(numStandardDeviationsChanged(double)), filter, SLOT(setNumStandardDeviations(double)));
}


void DialogFilter::addFilter()
{
	QString newType = ui->filterList->currentText();

	// Add a new filter based on the text of the selection
	if (newType == AverageDeviation::GetFilterName())
	{
		AddAverageDeviationFilter();
	}
	else if (newType == StandardDeviation::GetFilterName())
	{
		AddStandardDeviationFilter();
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
		ui->stackedWidget->removeWidget(widget);
		ui->stackedWidget->insertWidget(index+1, widget);
		ui->stackedWidget->setCurrentIndex(index+1);
	} else if (index != actionList.length() - 1){
		toolbar->removeAction(currentAction);
		toolbar->addAction(currentAction);
		actionList.move(index, index+1);
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
		ui->stackedWidget->removeWidget(widget);
		ui->stackedWidget->insertWidget(index-1, widget);
		ui->stackedWidget->setCurrentIndex(index-1);
	}
}


void DialogFilter::removeFilter()
{
	QAction *currentAction = actiongroup->checkedAction();
	int index = actionList.indexOf(currentAction);
	toolbar->removeAction(currentAction);
	actiongroup->removeAction(currentAction);
	actionList.removeOne(currentAction);
	ui->stackedWidget->removeWidget(ui->stackedWidget->widget(index));
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
	// Prevent the user from screwing things up
	ui->addFilterButton->setEnabled(false);

	// Show the progress bars
	ui->progressMain->setVisible(true);
	ui->progressSub->setVisible(true);
	ui->progressMain->setFormat("");
	ui->progressMain->setMaximum(actionList.length()*100);
	ui->progressMain->setValue(0);
	ui->progressSub->setValue(0);

	// Setup the first filter
	currentFilter = 0;
	Filter *filter = filterList.first();
	filter->setAutoDelete(false);

	// Set the cloud in the first filter
	filter->setInputCloud(cloud);

	// Start the first filter
	QThreadPool::globalInstance()->start(filter);

	// Setup the last filter
	Filter *lastFilter = filterList.last();
	connect(lastFilter, SIGNAL(filterFinished(PointCloudT*)), this, SIGNAL(filterComplete(PointCloudT*)));

}


void DialogFilter::filterError(QString error)
{
	// Hide the progress bars and enable the button
	ui->progressMain->setVisible(false);
	ui->progressSub->setVisible(false);
	ui->addFilterButton->setEnabled(true);

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
	ui->progressMain->setFormat("Filter Step " + QString::number(currentFilter+1));
}


void DialogFilter::filterProgress(int val)
{
	ui->progressMain->setValue(100*currentFilter + val);
	ui->progressSub->setValue(val);
}


void DialogFilter::filterFinished(PointCloudT *cloud)
{
	// Make the next filter the current filter
	++currentFilter;

	// Make sure there is a next filter
	if (currentFilter < filterList.size())
	{
		Filter *filter = filterList.at(currentFilter);
		filter->setAutoDelete(false);
		if (filter)
		{
			// Send the point cloud to the next filter
			filter->setInputCloud(cloud);
			QThreadPool::globalInstance()->start(filter);
		}
	} else {

		disconnect(filterList.last(), SIGNAL(filterFinished(PointCloudT*)), this, SIGNAL(filterComplete(PointCloudT*)));
		this->cloud = cloud;
		ui->progressMain->setVisible(false);
		ui->progressSub->setVisible(false);
		ui->addFilterButton->setEnabled(true);
	}
}
