cmake_minimum_required(VERSION 3.10)
project(TowerDefence)

set(CMAKE_CXX_STANDARD 17)

add_executable(TowerDefence
        driver.cpp
        tower.cpp
        critter.cpp
        CritterGroup.cpp
        mapgen.cpp
)
