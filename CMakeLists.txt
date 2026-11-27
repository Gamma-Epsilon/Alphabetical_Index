cmake_minimum_required(VERSION 3.16)
project(AlphabeticalIndexGui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.3 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(alphabetical_index_gui
    gui/main.cpp
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/mainwindow.ui
    sources/AlphabeticalIndex.cpp
)

target_include_directories(alphabetical_index_gui PRIVATE headers)
target_link_libraries(alphabetical_index_gui PRIVATE Qt6::Widgets)
set_target_properties(alphabetical_index_gui PROPERTIES WIN32_EXECUTABLE ON)