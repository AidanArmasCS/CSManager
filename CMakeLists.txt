cmake_minimum_required(VERSION 3.26)
project(MACCSManager)

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(MACCSManager main.cpp
        core/player.h
        core/player.cpp
        core/Trait.cpp
        core/Trait.h
        core/PlayerStyle.h
        core/MatchResult.h
        core/Team.cpp
        core/Team.h
        engine/RatingCalculator.cpp
        engine/RatingCalculator.h
        engine/Hltvcalculator.cpp
        engine/Hltvcalculator.h
        engine/Matchsimulator.cpp
        engine/Matchsimulator.h
        engine/Tournament.cpp
        engine/Tournament.h)
