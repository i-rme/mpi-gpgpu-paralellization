cmake_minimum_required(VERSION 3.16)

project(Test2_OpenCL)

find_package(OpenCL REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(Test2_OpenCL main.cpp)
target_link_libraries(Test2_OpenCL OpenCL::OpenCL)