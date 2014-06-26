#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QColor>

// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
#include <vtkCutter.h>
#include <vtkPlane.h>

// Data Types
#include "DataTypes.h"

// Filters
#include "Dialogs/DialogFilter.h"
#include "Dialogs/DialogGSDFilter.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:

		void	closeFile();
		void	filterCreate();
		void	filterGSD();
		void	openFile();
		void	openRecentFile();
		void	status(QString newStatus, int time);
		void	useColorGradient();
		void	useNewCloud(PointCloudT *newCloud);

	protected:
		// Point cloud viewer
		pcl::visualization::PCLVisualizer	*viewer;

		// Visible point cloud
		PointCloudT		*cloud;
		PointCloudT::Ptr	cloudPtr;


	private:
		Ui::MainWindow *ui;

		void	loadFile(QString fileLocation);
		void	loadSettings();
		void	saveSettings();
		void	updateRecentFiles();

		// Action Menu Items
		QAction	*recentFiles[MaxRecentFiles];
		QAction	*recentProjectsFiles;

		// Modeless Dialog Windows
		DialogGSDFilter	*gsdDialog;
};

#endif // MAINWINDOW_H
