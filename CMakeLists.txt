# CMakeLists.txt for CrossWordPuzzleGame project.

cmake_minimum_required(VERSION 3.31)

project(CrossWordPuzzleGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(wxBUILD_SHARED OFF)
set(CROSSWORD_BUILD_TEST OFF)
set(HAPPYMATH_BUILD_TEST OFF)

add_subdirectory(GameApp)
add_subdirectory(Dependencies/wxWidgets)
add_subdirectory(Dependencies/CrossWordPuzzleGenerator)
add_subdirectory(Dependencies/FontSystem)
add_subdirectory(Dependencies/HappyMath)