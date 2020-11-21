cmake_minimum_required(VERSION 3.17)
project(teru_dap)

set(CMAKE_CXX_STANDARD 11)

add_library(teru_dap teru_dap.cpp teru_dap.h config.h)