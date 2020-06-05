cmake_minimum_required(VERSION 3.15)
project(GPGPU)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCL REQUIRED)

add_executable(GPGPU main.cpp)
target_link_libraries(GPGPU OpenCL::OpenCL)