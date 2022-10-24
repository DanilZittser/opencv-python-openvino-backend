cmake_minimum_required(VERSION 3.22)
project(opencv_python_openvino_backend)

set(CMAKE_CXX_STANDARD 14)

set(OpenCV_DIR $ENV{OpenCV_DIR})
find_package(OpenCV REQUIRED)

add_executable(opencv_python_openvino_backend samples/main.cpp)
target_link_libraries(opencv_python_openvino_backend ${OpenCV_LIBS})