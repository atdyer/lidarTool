#include "FormRegularGrid.h"
#include "../../lidar-build/ui_FormRegularGrid.h"

FormRegularGrid::FormRegularGrid(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormRegularGrid)
{
	ui->setupUi(this);

	connect(ui->numRows, SIGNAL(valueChanged(int)), this, SIGNAL(numRowsChanged(int)));
	connect(ui->numCols, SIGNAL(valueChanged(int)), this, SIGNAL(numColsChanged(int)));
}

FormRegularGrid::~FormRegularGrid()
{
	delete ui;
}
