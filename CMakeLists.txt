cmake_minimum_required(VERSION 3.5.1)
project(iovirta_iot_serveri)

include_directories(inc)

set(SOURCES src/main.cpp src/conf/conf.cpp src/video/camera.cpp src/video/motion.cpp src/network/fcm_server.cpp)

find_package(OpenCV REQUIRED)

add_executable(iovirta_iot ${SOURCES})
target_link_libraries(iovirta_iot ${OpenCV_LIBS} -pthread -lboost_system)
set_property(TARGET iovirta_iot PROPERTY CXX_STANDARD 14)
