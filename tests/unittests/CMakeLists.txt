# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

project(GTlabPython-UnitTests)

if (NOT TARGET Qt5::Core)
    find_package(Qt5 REQUIRED COMPONENTS Core)
endif()


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()


add_executable(GTlabPythonUnitTest
    main.cpp
    test_helper.h
    test_variantconvert.cpp
    test_contextconfig.cpp
)

target_compile_definitions(GTlabPythonUnitTest
    PRIVATE GT_MODULE_ID="Python Unit Tests"
)

target_link_libraries(GTlabPythonUnitTest PRIVATE GTlab::Core GTlab::Python gtest Qt5::Core)

include(GoogleTest)
gtest_discover_tests(GTlabPythonUnitTest TEST_PREFIX "PythonModule." DISCOVERY_MODE PRE_TEST)
