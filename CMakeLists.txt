cmake_minimum_required(VERSION 3.15)
project(MultExperiment)

set(CMAKE_CXX_STANDARD 14)
include_directories("/Users/arina/anaconda3/pkgs/python-3.7.7-hc70fcce_0_cpython/include/python3.7m")

find_package(PythonLibs REQUIRED)

add_executable(MultExperiment main.cpp)

target_include_directories(MultExperiment PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(MultExperiment PRIVATE ${PYTHON_LIBRARIES})