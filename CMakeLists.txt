cmake_minimum_required(VERSION 3.23)
project(FileFinder)

set(CMAKE_CXX_STANDARD 20)


add_library(FileFinder STATIC library.cpp library.h Core.h)

add_subdirectory(ThreadPool)
add_subdirectory(OneTask)
