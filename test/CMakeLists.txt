
cmake_minimum_required(VERSION 2.8)


SET(PRJ test)

project(${PRJ})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(../)
include_directories(../modules)


set(
SRC
test.cpp
test_reader.cpp
test_window_zoom.cpp
test_seedwidgets.cpp
)

set(
HEAD
)


add_executable(${PRJ} ${SRC} ${HEAD})
qt5_use_modules(${PRJ} Core Gui)

target_link_libraries(${PRJ} ${VTK_LIBRARIES})
target_link_libraries(${PRJ} ${ITK_LIBRARIES})
target_link_libraries(${PRJ} Reconstruct_module)


