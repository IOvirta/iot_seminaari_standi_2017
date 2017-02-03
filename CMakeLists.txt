cmake_minimum_required(VERSION 3.5.1)
project(iovirta_iot_serveri)

include_directories(inc)

set(SOURCES src/main.cpp src/conf/conf.cpp)

add_executable(iovirta_iot ${SOURCES})
set_property(TARGET iovirta_iot PROPERTY CXX_STANDARD 14)
