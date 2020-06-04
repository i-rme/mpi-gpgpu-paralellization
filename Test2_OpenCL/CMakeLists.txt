cmake_minimum_required(VERSION 3.15)
project(Test2_OpenCL)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCL REQUIRED)

add_executable(Test2_OpenCL main.cpp)
target_link_libraries(Test2_OpenCL OpenCL::OpenCL)