cmake_minimum_required(VERSION 3.13)
project(PSINS_1_0)

set(CMAKE_CXX_STANDARD 14)

add_executable(PSINS_1_0 Main_SINSGPS.cpp KFApp.cpp KFApp.h PSINS.cpp PSINS.h)