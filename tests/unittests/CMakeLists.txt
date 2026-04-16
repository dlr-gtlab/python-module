# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

project(GTlabPython-UnitTests)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT TARGET Qt5::Core AND NOT TARGET Qt6::Core)
    include(RequireQt)
    require_qt(COMPONENTS Core)
endif()

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()


add_executable(GTlabPythonUnitTest
    main.cpp
    test_helper.h
    test_variantconvert.cpp
    test_codegen.cpp
    test_contextconfig.cpp
)

target_compile_definitions(GTlabPythonUnitTest
    PRIVATE GT_MODULE_ID="Python Unit Tests"
)

target_link_libraries(GTlabPythonUnitTest PRIVATE GTlab::Core GTlab::Python gtest Qt${QT_VERSION_MAJOR}::Core)

include(GoogleTest)
gtest_discover_tests(GTlabPythonUnitTest TEST_PREFIX "PythonModule." DISCOVERY_MODE PRE_TEST)
