cmake_minimum_required(VERSION 3.17)
project(Fifteen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -municode")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgdiplus")
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lshlwapi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(Fifteen main.cpp Game.h Game.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")