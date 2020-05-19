cmake_minimum_required(VERSION 3.15)

project(GPGPU)

find_package(OpenCL REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(GPGPU main.cpp)
target_link_libraries(GPGPU OpenCL::OpenCL)