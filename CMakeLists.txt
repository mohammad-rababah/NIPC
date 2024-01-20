cmake_minimum_required(VERSION 3.27)
project(NIPC)

set(CMAKE_CXX_STANDARD 17)

add_executable(NIPC main.cpp
        NIPC.cpp
        NIPC.h
        NIPCManeger.cpp
        NIPCManeger.h
        configs.h)
