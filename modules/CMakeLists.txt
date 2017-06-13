
cmake_minimum_required(VERSION 2.8)


SET(PRJ Reconstruct_module)

project(${PRJ})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(
SRC
interactor_style.cpp
model3d_viewer.cpp
image_viewer.cpp

vtkSeedWidgetx.cpp
globalEventCallback.cpp
vtkRendererx.cpp
vtksplineinterpolate.cpp
)

set(
HEAD
module_config.h
dicom_retrieve.h
interactor_style.h
model3d_viewer.h
image_viewer.h

vtkSeedWidgetx.h
globalEventCallback.h
vtkRendererx.h
vtksplineinterpolate.h
)

add_library(${PRJ} ${SRC} ${HEAD})

target_link_libraries(${PRJ} ${VTK_LIBRARIES})
target_link_libraries(${PRJ} ${ITK_LIBRARIES})



