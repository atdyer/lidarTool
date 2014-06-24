#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QColor>

// Point Cloud Library
#include <pcl/common/common.h>
#include <pcl/common/transforms.h>
#include <pcl/common/geometry.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>

// LAS File Reading Library
#include <liblas/liblas.hpp>
#include <liblas/reader.hpp>
#include <liblas/header.hpp>
#include <fstream>

// Eigen Library
#include <eigen3/Eigen/Eigen>

// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
#include <vtkCutter.h>
#include <vtkPlane.h>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

namespace Ui {
	class MainWindow;
}

enum {MaxRecentFiles = 5};

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	public slots:

		void	closeFile();
		void	openFile();
		void	openRecentFile();
		void	status(QString newStatus, int time);
		void	useColorGradient();

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

		QAction	*recentFiles[MaxRecentFiles];
		QAction	*recentProjectsFiles;
};

#endif // MAINWINDOW_H
