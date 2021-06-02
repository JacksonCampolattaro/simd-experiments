cmake_minimum_required(VERSION 3.13)
project(ray-bbox-intersections)
set(CMAKE_CXX_STANDARD 17)

add_executable(main
        main.cpp
        vector3.h
        ray.h
        bbox.h)