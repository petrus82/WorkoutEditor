cmake_minimum_required(VERSION 3.30)

option(QtTests "Enable Qt Tests" OFF)
set(EXECUTABLE_NAME "workoutchart")
set(PLUGIN_NAME "WorkoutChartModule")
set(QML_NAME "WorkoutChart_qml_module")
set(TEST_NAME "workoutcharttests")
set(GARMIN_DIR "${CMAKE_BINARY_DIR}/Garmin SDK/cpp/")
message("Garmin Dir: ${GARMIN_DIR}")

# Qt settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/Zc:__cplusplus")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick Qml Test)
qt6_standard_project_setup()

file (GLOB SOURCE_FILES
  workoutchart.cpp
  workoutchart.hpp
  ${CMAKE_SOURCE_DIR}/WorkoutLib/*.hpp
)
message("Source files: " ${SOURCE_FILES})

qt_add_qml_module(${PLUGIN_NAME}
   URI WorkoutChart
   VERSION 1.0
   DEPENDENCIES Core QtQuick Qml Gui WorkoutLib
   SOURCES 
    workoutchart.cpp
    workoutchart.hpp
   OUTPUT_DIRECTORY WorkoutChart
   QML_FILES
     main.ui.qml
)
qt_query_qml_module(${PLUGIN_NAME}
 TARGET_PATH 
  module_target_path
)
qt_add_executable(${EXECUTABLE_NAME}
  ${SOURCE_FILES}
  main.cpp
  resources.qrc
)

add_library(${QML_NAME}
   ${SOURCE_FILES}
)

link_directories(${CMAKE_BINARY_DIR}/build)

target_link_libraries(${PLUGIN_NAME} PUBLIC
  Qt6::Quick
  WorkoutLib
)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
  #Qt6::Core
  Qt6::Quick
  #Qt6::Qml
  Qt6::Gui      # QColor
  WorkoutLib
)

target_include_directories(${QML_NAME} PUBLIC
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/WorkoutLib"
    "${GARMIN_DIR}"
)
target_include_directories(${PLUGIN_NAME} PUBLIC
    ${Qt6Gui_INCLUDE_DIRS}
    "${CMAKE_SOURCE_DIR}/WorkoutLib"
    "${GARMIN_DIR}"
)

target_include_directories(${EXECUTABLE_NAME} PUBLIC
    "${GARMIN_DIR}"
)

link_directories(${CMAKE_BINARY_DIR})

if(QtTests)

  qt_add_executable(${TEST_NAME} 
    ${SOURCE_FILES}
    chart-testing.cpp
  )

  qt_add_resources(${TEST_NAME}
    resources.qrc
  )
  target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/WorkoutLib
  )

  target_link_libraries(${TEST_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::Test
    WorkoutLib
  )

endif()

install(TARGETS ${EXECUTABLE_NAME} 
    DESTINATION ${CMAKE_BINARY_DIR}/build
    COMPONENT bin
)