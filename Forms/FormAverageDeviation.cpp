#include "FormAverageDeviation.h"
#include "../../lidar-build/ui_FormAverageDeviation.h"

FormAverageDeviation::FormAverageDeviation(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormAverageDeviation)
{
	ui->setupUi(this);

	connect(ui->tendencyCombo, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(tendencyMeasureChanged(QString)));
	connect(ui->scalingFactor, SIGNAL(valueChanged(double)), this, SIGNAL(scalingFactorChanged(double)));
}

FormAverageDeviation::~FormAverageDeviation()
{
	delete ui;
}
