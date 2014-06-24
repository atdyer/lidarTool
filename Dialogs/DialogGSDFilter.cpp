#include "DialogGSDFilter.h"
#include "../../lidar-build/ui_DialogGSDFilter.h"

DialogGSDFilter::DialogGSDFilter(PointCloudT *pointCloud, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogGSDFilter),
	cloud(pointCloud)
{
	ui->setupUi(this);

	ui->progressBar->setVisible(false);

	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyFilter()));
}

DialogGSDFilter::~DialogGSDFilter()
{
	delete ui;
}


void DialogGSDFilter::applyFilter()
{
	// QThreadPool will delete the filter when it's finished
	GSDFilter *filter = new GSDFilter(cloud, 0);

	connect(filter, SIGNAL(started()), this, SLOT(showProgress()));
	connect(filter, SIGNAL(progress(QString,int)), this, SLOT(progress(QString,int)));
	connect(filter, SIGNAL(finished()), this, SLOT(hideProgress()));

	filter->SetGridDimensions(ui->rowsSpinBox->value(), ui->columnSpinBox->value());

	QThreadPool::globalInstance()->start(filter);
}


void DialogGSDFilter::hideProgress()
{
	ui->progressBar->setVisible(false);
	ui->applyButton->setEnabled(true);
}


void DialogGSDFilter::progress(QString message, int value)
{
	ui->progressBar->setFormat(message);
	ui->progressBar->setValue(value);
}


void DialogGSDFilter::showProgress()
{
	ui->progressBar->setVisible(true);
	ui->applyButton->setEnabled(false);
}
