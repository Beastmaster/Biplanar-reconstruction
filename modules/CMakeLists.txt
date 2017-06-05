
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
image_viewer.cpp
seedwidget.cpp
)

set(
HEAD
module_config.h
dicom_retrieve.h
interactor_style.h
custom_callbacks.h
image_viewer.h
seedwidget.h
)

add_library(${PRJ} ${SRC} ${HEAD})

target_link_libraries(${PRJ} ${VTK_LIBRARIES})
target_link_libraries(${PRJ} ${ITK_LIBRARIES})



