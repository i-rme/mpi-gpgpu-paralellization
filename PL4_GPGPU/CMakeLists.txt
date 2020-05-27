cmake_minimum_required(VERSION 3.15)
project(PL4_GPGPU)

find_package(OpenCL REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(PL4_GPGPU main.cpp)
target_link_libraries(PL4_GPGPU OpenCL::OpenCL)