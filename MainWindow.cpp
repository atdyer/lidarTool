#include "MainWindow.h"
#include "../lidar-build/ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	gsdDialog = 0;

	// Load settings
	loadSettings();

	// Create the visualizer
	viewer = new pcl::visualization::PCLVisualizer("viewer", false);
	ui->viewport->SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(ui->viewport->GetInteractor(), ui->viewport->GetRenderWindow());
	ui->viewport->update();

	// Create the point cloud
	cloud = new PointCloudT;
	cloudPtr.reset(cloud);

	// Connect UI actions
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
	connect(ui->actionGradient, SIGNAL(triggered()), this, SLOT(useColorGradient()));
	connect(ui->actionCreateFilter, SIGNAL(triggered()), this, SLOT(filterCreate()));
	connect(ui->actionGSDFilter, SIGNAL(triggered()), this, SLOT(filterGSD()));

}

MainWindow::~MainWindow()
{
	delete ui;

	if (viewer)
		delete viewer;
}


void MainWindow::closeFile()
{
	cloudPtr->clear();
	viewer->removeAllPointClouds();
	viewer->resetCamera();
	ui->viewport->update();
}


void MainWindow::filterCreate()
{
	DialogFilter *dlg = new DialogFilter(this);
	if (cloud)
		dlg->SetCloud(cloud);
	connect(dlg, SIGNAL(filterComplete(PointCloudT*)), this, SLOT(useNewCloud(PointCloudT*)));
	dlg->exec();
}


void MainWindow::filterGSD()
{
	if (cloud->points.size() > 0)
	{
		if (!gsdDialog)
		{
			gsdDialog = new DialogGSDFilter(cloud, this);
		}

		gsdDialog->show();
		gsdDialog->raise();
		gsdDialog->activateWindow();
	}
}


void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open LIDAR File"),
							"/home/tristan/", tr("LIDAR Data Files (*.las *.laz)"));
	loadFile(fileName);
}


void MainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		loadFile(action->data().toString());
	}
}


void MainWindow::status(QString newStatus, int time)
{
	ui->statusBar->showMessage(newStatus, time);
}


void MainWindow::useColorGradient()
{
	float minEle = 99999.0;
	float maxEle = 0.0;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		if (cloud->points[i].z > maxEle)
			maxEle = cloud->points[i].z;
		else if (cloud->points[i].z < minEle)
			minEle = cloud->points[i].z;

	}

	float eleRange = maxEle - minEle;
	for (int i=0; i<cloud->points.size(); ++i)
	{
		QColor c = QColor::fromHsv(240 - (int)240*(cloud->points[i].z - minEle)/eleRange, 255, 255);
		cloud->points[i].r = c.red();
		cloud->points[i].g = c.green();
		cloud->points[i].b = c.blue();
	}

	viewer->updatePointCloud(cloudPtr);
	ui->viewport->update();
}


void MainWindow::useNewCloud(PointCloudT *newCloud)
{
	// Clear the existing point cloud
	viewer->removeAllPointClouds();

	// Use the new point cloud
	cloud = newCloud;
	cloudPtr.reset(newCloud);

	// Add the cloud to the viewer
	viewer->addPointCloud(cloudPtr, "cloud");
	viewer->resetCamera();
	ui->viewport->update();
}


void MainWindow::loadFile(QString fileLocation)
{
	if (QFile(fileLocation).exists())
	{
		status("Loading file: " + fileLocation, 1000);

		// Update recent files list
		QSettings settings;
		QStringList files = settings.value("recentFilesList").toStringList();
		files.removeAll(fileLocation);
		files.prepend(fileLocation);
		while (files.size() > MaxRecentFiles)
			files.removeLast();
		settings.setValue("recentFilesList", files);
		updateRecentFiles();

		// Create the input stream
		std::ifstream ifs;
		ifs.open(fileLocation.toStdString().data(), std::ios::in | std::ios::binary);

		// Create reader factory
		liblas::ReaderFactory f;
		liblas::Reader reader = f.CreateWithStream(ifs);

		// Read the file header
		liblas::Header const& header = reader.GetHeader();

		// Clear the existing point cloud
		viewer->removeAllPointClouds();
		cloudPtr->clear();

		// Resized the point cloud and add the points
		cloudPtr->points.resize(header.GetPointRecordsCount());
		int currentPoint = 0;
		while(reader.ReadNextPoint())
		{
			liblas::Point const& p = reader.GetPoint();
			cloudPtr->points[currentPoint].x = p.GetX();
			cloudPtr->points[currentPoint].y = p.GetY();
			cloudPtr->points[currentPoint].z = p.GetZ();
			cloudPtr->points[currentPoint].r = 255;
			cloudPtr->points[currentPoint].g = 255;
			cloudPtr->points[currentPoint].b = 255;
			++currentPoint;
		}

		// Add the point cloud to the viewer
		viewer->addPointCloud(cloudPtr, "cloud");
		viewer->resetCamera();
		ui->viewport->update();
	}
}


void MainWindow::loadSettings()
{
	QSettings settings;

	// Load geometry
	restoreGeometry(settings.value("window/geometry").toByteArray());
	restoreState(settings.value("window/windowState").toByteArray());

	// Load recent files
	QList<QAction*> actionList;
	for (int i=0; i<MaxRecentFiles; ++i)
	{
		recentFiles[i] = new QAction(this);
		recentFiles[i]->setVisible(false);
		connect(recentFiles[i], SIGNAL(triggered()),
			this, SLOT(openRecentFile()));
		actionList.append(recentFiles[i]);
	}
	recentProjectsFiles = new QAction(this);
	recentProjectsFiles->setSeparator(true);
	recentProjectsFiles->setVisible(false);
	actionList.append(recentProjectsFiles);

	ui->menuFile->insertActions(ui->actionClose, actionList);

	updateRecentFiles();
}


void MainWindow::saveSettings()
{
	QSettings settings;

	settings.setValue("window/geometry", saveGeometry());
	settings.setValue("window/windowState", saveState());
}


void MainWindow::updateRecentFiles()
{
	QSettings settings;
	QStringList files = settings.value("recentFilesList").toStringList();
	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i=0; i<numRecentFiles; ++i)
	{
		QString text = tr("&%1 %2").arg(i+1).arg(QFileInfo(files[i]).fileName());
		recentFiles[i]->setText(text);
		recentFiles[i]->setData(files[i]);
		recentFiles[i]->setVisible(true);
	}

	for (int i=numRecentFiles; i<MaxRecentFiles; ++i)
		recentFiles[i]->setVisible(false);

	recentProjectsFiles->setVisible(numRecentFiles > 0);
}

























