#include "FormStandardDeviation.h"
#include "../../lidar-build/ui_FormStandardDeviation.h"

FormStandardDeviation::FormStandardDeviation(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormStandardDeviation)
{
	ui->setupUi(this);

	connect(ui->numStandardDeviations, SIGNAL(valueChanged(double)), this, SIGNAL(numStandardDeviationsChanged(double)));
}

FormStandardDeviation::~FormStandardDeviation()
{
	delete ui;
}
