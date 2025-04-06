cmake_minimum_required(VERSION 3.10)
project(SudokuGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)  

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    sudokuGameQt.cpp
    sudokuGameQt.h
    sudokuLogic.cpp
    sudokuLogic.h
    resources.qrc
    sudokuGameQt.qrc
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
