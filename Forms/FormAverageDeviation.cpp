#include "FormAverageDeviation.h"
#include "../../lidar-build/ui_FormAverageDeviation.h"

FormAverageDeviation::FormAverageDeviation(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormAverageDeviation)
{
	ui->setupUi(this);

	ui->roundingLabel->setVisible(false);
	ui->roundingValue->setVisible(false);

	connect(ui->tendencyCombo, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(tendencyMeasureChanged(QString)));
	connect(ui->tendencyCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(tendencyChanged(QString)));
	connect(ui->scalingFactor, SIGNAL(valueChanged(double)), this, SIGNAL(scalingFactorChanged(double)));
	connect(ui->roundingValue, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(roundingValueChanged(QString)));
}

FormAverageDeviation::~FormAverageDeviation()
{
	delete ui;
}


void FormAverageDeviation::tendencyChanged(QString val)
{
	if (QString::compare(val, "Mode", Qt::CaseInsensitive) == 0)
	{
		ui->roundingLabel->setVisible(true);
		ui->roundingValue->setVisible(true);
		emit roundingValueChanged(ui->roundingValue->currentText());
	} else {
		ui->roundingLabel->setVisible(false);
		ui->roundingValue->setVisible(false);
	}
}
