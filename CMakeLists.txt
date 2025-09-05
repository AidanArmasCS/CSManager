cmake_minimum_required(VERSION 3.26)
project(CSManager)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CSManager
        main.cpp
        core/Match.cpp
        core/Match.h
        core/Player.cpp
        core/Player.h
        core/Team.cpp
        core/Team.h
        core/Trait.h
        core/Trait.cpp
        Tournament.cpp
        Tournament.h
        Tournament.h
        simulation/MatchSimulator.cpp
        simulation/MatchSimulator.h
        simulation/StatTracker.cpp
        simulation/StatTracker.h
        data/DataLoader.cpp
        data/DataLoader.h
        ui/MenuManager.cpp
        ui/MenuManager.h
        ui/DisplayUtils.cpp
        ui/DisplayUtils.h)
