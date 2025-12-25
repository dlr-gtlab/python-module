#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
######################################################################

cmake_minimum_required(VERSION 3.15)
project(ResourceModule VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

option(BUILD_UNITTESTS "Build Unittests" OFF)

find_package(Qt5 COMPONENTS Core Widgets Xml REQUIRED)

add_subdirectory(src)

if (BUILD_UNITTESTS)
    enable_testing()
    add_subdirectory(tests/unittests)
endif ()
