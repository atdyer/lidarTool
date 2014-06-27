cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project      (lidar)
find_package (Qt4 REQUIRED)
find_package (VTK REQUIRED)
find_package (PCL 1.7.1 REQUIRED)

include_directories (${PCL_INCLUDE_DIRS})
link_directories    (${PCL_LIBRARY_DIRS} /usr/local/lib/)
add_definitions     (${PCL_DEFINITIONS})

set  (project_SOURCES main.cpp 
		      MainWindow.cpp 
		      Filters/Filter.cpp
		      Filters/AverageDeviation.cpp
		      Filters/StandardDeviation.cpp
		      Forms/FormAverageDeviation.cpp
		      Forms/FormStandardDeviation.cpp
		      Forms/FormRegularGrid.cpp
		      Grids/Grid.cpp
		      Grids/GridCell.cpp
		      Grids/RegularGrid.cpp
		      Dialogs/DialogFilter.cpp)
set  (project_HEADERS MainWindow.h
		      DataTypes.h
		      Filters/Filter.h
		      Filters/AverageDeviation.h
		      Filters/StandardDeviation.h
		      Forms/FormAverageDeviation.h
		      Forms/FormStandardDeviation.h
		      Forms/FormRegularGrid.h
		      Grids/Grid.h
		      Grids/GridCell.h
		      Grids/RegularGrid.h
		      Dialogs/DialogFilter.h)
set  (project_FORMS   MainWindow.ui
		      Dialogs/DialogFilter.ui
		      Forms/FormAverageDeviation.ui
		      Forms/FormStandardDeviation.ui
		      Forms/FormRegularGrid.ui)
set  (VTK_LIBRARIES   vtkRendering vtkGraphics vtkHybrid QVTK)

QT4_WRAP_CPP (project_HEADERS_MOC   ${project_HEADERS})
QT4_WRAP_UI  (project_FORMS_HEADERS ${project_FORMS})

INCLUDE         (${QT_USE_FILE})
ADD_DEFINITIONS (${QT_DEFINITIONS})

ADD_EXECUTABLE  (lidar ${project_SOURCES}
                       ${project_FORMS_HEADERS}
                       ${project_HEADERS_MOC})

TARGET_LINK_LIBRARIES (lidar ${QT_LIBRARIES} ${PCL_LIBRARIES} ${VTK_LIBRARIES} las laszip)

