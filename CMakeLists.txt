
cmake_minimum_required(VERSION 2.8)


SET(PRJ Reconstruct)

project(${PRJ})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Qt5Widgets REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
include_directories(modules)
add_subdirectory(test)
add_subdirectory(modules)

set( UI_FILES
mainWindow.ui
)

set( QT_HEAD
mainWindow.h
)


set(
SRC
main.cpp
mainWindow.cpp
)

set(
HEAD
)



qt5_wrap_ui(UISrcs ${UI_FILES})
qt5_wrap_cpp(MOCSrcs ${QT_HEAD})
add_executable(${PRJ} ${SRC} ${HEAD} ${UISrcs} ${QT_HEAD} ${MOCSrcs} )
qt5_use_modules(${PRJ} Core Gui)

target_link_libraries(${PRJ} Reconstruct_module)
target_link_libraries(${PRJ} ${VTK_LIBRARIES})
target_link_libraries(${PRJ} ${ITK_LIBRARIES})



