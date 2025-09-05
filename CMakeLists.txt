cmake_minimum_required(VERSION 3.26)
project(CSManager)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CSManager
        main.cpp
        Match.cpp
        Match.h
        Player.cpp
        Player.h
        Team.cpp
        Team.h
        Trait.h
        Trait.cpp
        Tournament.cpp
        Tournament.h
        Tournament.h)
