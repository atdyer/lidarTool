#ifndef FORMSTANDARDDEVIATION_H
#define FORMSTANDARDDEVIATION_H

#include <QWidget>

namespace Ui {
	class FormStandardDeviation;
}

class FormStandardDeviation : public QWidget
{
		Q_OBJECT

	public:
		explicit FormStandardDeviation(QWidget *parent = 0);
		~FormStandardDeviation();

	private:
		Ui::FormStandardDeviation *ui;

	signals:

		void	numStandardDeviationsChanged(double);
};

#endif // FORMSTANDARDDEVIATION_H
