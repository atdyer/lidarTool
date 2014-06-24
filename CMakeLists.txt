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
		      Filters/GSDFilter.cpp 
		      DataStructures/Grid.cpp
		      Dialogs/DialogGSDFilter.cpp)
set  (project_HEADERS MainWindow.h
		      DataTypes.h
		      Filters/GSDFilter.h
		      DataStructures/Grid.h
		      Dialogs/DialogGSDFilter.h)
set  (project_FORMS   MainWindow.ui
		      Dialogs/DialogGSDFilter.ui)
set  (VTK_LIBRARIES   vtkRendering vtkGraphics vtkHybrid QVTK)

QT4_WRAP_CPP (project_HEADERS_MOC   ${project_HEADERS})
QT4_WRAP_UI  (project_FORMS_HEADERS ${project_FORMS})

INCLUDE         (${QT_USE_FILE})
ADD_DEFINITIONS (${QT_DEFINITIONS})

ADD_EXECUTABLE  (lidar ${project_SOURCES}
                       ${project_FORMS_HEADERS}
                       ${project_HEADERS_MOC})

TARGET_LINK_LIBRARIES (lidar ${QT_LIBRARIES} ${PCL_LIBRARIES} ${VTK_LIBRARIES} las laszip)

