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
)

target_link_libraries(GTlabPythonUnitTest PRIVATE GTlab::Python gtest Qt5::Core)


