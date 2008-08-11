cmake_minimum_required(VERSION 2.6)

project(segmentation)

find_package(Boost)

find_package(Qt4 REQUIRED)
SET(QT_USE_QT3SUPPORT true)
INCLUDE(${QT_USE_FILE})



QT4_WRAP_UI(segmentation_UIS_H src/segmentationbase.ui)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} src)

QT4_WRAP_CPP(segmentation_MOC_SRCS src/segmentation.h)


ADD_EXECUTABLE(segmentation 
	src/main.cpp src/graphcut.cpp src/segmentation.cpp 
	src/external/freeimpl/freegraph.cpp 
	src/external/kolmo_maxflow/adjacency_list/graph.cpp src/external/kolmo_maxflow/adjacency_list/maxflow.cpp 
	${segmentation_MOC_SRCS} ${segmentation_UIS_H})

TARGET_LINK_LIBRARIES(segmentation ${QT_LIBRARIES})
