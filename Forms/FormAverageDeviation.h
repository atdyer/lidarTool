#ifndef FORMAVERAGEDEVIATION_H
#define FORMAVERAGEDEVIATION_H

#include <QWidget>
#include <QComboBox>
#include <QDoubleSpinBox>

namespace Ui {
	class FormAverageDeviation;
}

class FormAverageDeviation : public QWidget
{
		Q_OBJECT

	public:
		explicit FormAverageDeviation(QWidget *parent = 0);
		~FormAverageDeviation();

	private:
		Ui::FormAverageDeviation *ui;

	signals:

		void	tendencyMeasureChanged(QString);
		void	scalingFactorChanged(double);
};

#endif // FORMAVERAGEDEVIATION_H
